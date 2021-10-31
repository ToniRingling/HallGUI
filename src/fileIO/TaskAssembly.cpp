#include "TaskAssembly.h"

#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <unordered_map>
#include <codecvt>
#include <algorithm>

#include "../nodes/NodeComponents.h"
#include "../customUI/SmallPreview.h"
#include "../customUI/ProjectProperties.h"
#include "../customUI/GraphEditor.h"
#include "../customUI/LogField.h"
#include "../nodes/optionCores/OptionCore.h"

using namespace node_components;

// How to start hallgrimJS from command line in windows or linux: python -c "import hallgrimJS; hallgrimJS.parseme()"
// might need to replace python with python3 or whatever is appropriate


namespace{
    const std::string nodeBasePrefix = "GrimHallNode";
    const std::string nodeBaseSuffix = "Object";
    const std::string nodeRunningFunction = "GrimhallRunNode";
    const std::string nodeInitFunction = "GrimhallInitNode";
    const std::string nodeUIDPrefix = "GrimHallNode";
    const std::string nodeUIDSuffix = "ID";
}

GraphEditor* TaskAssembly::graphEditor;
ProjectProperties* TaskAssembly::projectProperties;
SmallPreview* TaskAssembly::preview;
LogField* TaskAssembly::logField;

void TaskAssembly::assemble(){
    logField->reset();

    if(checkForCycle()){
        logField->log(LogMessageTypes::error, "Task graph contains cycles, task likely to get stuck in a loop and freeze.");
    }

    wxFileDialog* savePathDialog = new wxFileDialog(NULL, "Build project", wxEmptyString, wxEmptyString, "*.py", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if(savePathDialog->ShowModal() == wxID_CANCEL){
        delete savePathDialog;
        return;
    }
    std::string filePath = savePathDialog->GetPath().ToStdString();
    delete savePathDialog;

    //// preparation phase
    // assign numbers to all nodes
    std::unordered_map<Node*, std::string> nodeNumbering;
    {
        int nodeCounter = 1;
        for(Node* node : graphEditor->nodes){
            nodeNumbering[node] = std::to_string(nodeCounter);
            nodeCounter++;
        }
    }

    // read all answers (for format see ProjectProperties)
    std::vector<std::vector<std::pair<std::string, std::string>>> answers;

    {
        std::stringstream answerStream(projectProperties->answers);
        std::string segmentBuffer;
        std::vector <std::string> answerLines;
        while(std::getline(answerStream, segmentBuffer, '\n')){ // separate lines
            answerLines.push_back(segmentBuffer);
        }
        answers.push_back(std::vector<std::pair<std::string, std::string>>());
        for(std::string line : answerLines){
            if(std::all_of(line.begin(), line.end(), isspace)){ // line is just whitespace
                answers.push_back(std::vector<std::pair<std::string, std::string>>()); // so we are now looking at the next answer gap
            }
            else{ // otherwise we got a new answer for this gap
                std::stringstream lineStream(line);
                std::string answer;
                std::string points;
                std::getline(lineStream, answer, ':');
                std::getline(lineStream, points);
                answers.back().push_back(std::pair<std::string, std::string>(answer, points));
            }
        }
        std::vector<std::vector<std::pair<std::string, std::string>>>::iterator answerIt = answers.begin();
        while(answerIt != answers.end()){
            if((*answerIt).size() == 0){
                answerIt = answers.erase(answerIt);
            }
            else{
                answerIt++;
            }
        }
    }

    //// assembly phase

    std::ofstream writer(filePath);

    writer << "# -*- coding: latin-1 -*-" << "\n"; // stop-gap measure until I can figure out how to get it to write real utf8
    writer << "meta = {" << "\n"
    << "'author': '<your name>'," << "\n"
    << "'title': '" + projectProperties->taskName + "'," << "\n"
    << "'type': 'javaScript'," << "\n"
    << "'instances': 1," << "\n"
    << "}" << "\n" << "\n";

    writer << "task = \"\"\"" << "\n";
    std::map<int, Node*>* taskNodeOrder = preview->getNodePositions();
    for(std::pair<int, Node*> it : *taskNodeOrder){
        Node* node = it.second;
        std::string task = *(node->task);
        fillPlaceholders(task, node, nodeNumbering[node]);
        writer << "\n" << task << "\n";
    }
    delete taskNodeOrder;
    writer << "\"\"\"" << "\n" << "\n";

    writer << "correctAnswers = [";
    for(std::vector<std::pair<std::string, std::string>> gap : answers){
        writer << "\"\"\"";
        for(std::pair<std::string, std::string> answer : gap){
            writer << answer.first << " : " << answer.second << "\n";
        }
        writer << "\"\"\"," << "\n";
    }

    writer << "]" << "\n" << "\n";

    writer << "appendedData = \"\"\"" << "\n" << "\n" << "\"\"\"" << "\n" << "\n";

    writer << "javaScriptCode = r\"\"\"" << "\n";

    for(Node* node : graphEditor->nodes){
        writer << "//" << *node->getName() << "\n";
        writer << "var " << (nodeBasePrefix + nodeNumbering.at(node) + nodeBaseSuffix) << " = {" << "\n"; // object Declaration start
        writer << "\t" << nodeInitFunction << " : function(){" << "\n"; // initializing function start
        std::string initScript = *(node->initScript);
        fillPlaceholders(initScript, node, nodeNumbering[node]);
        std::stringstream initScriptStream(initScript);
        std::string initScriptBuffer;
        while(getline(initScriptStream, initScriptBuffer)){
            writer << "" << initScriptBuffer << "\n";
        }
        writer << "\t}," << "\n"; // end of initializing function
        fillPlaceholders(initScript, node, nodeNumbering[node]);
        writer << "\t" << nodeRunningFunction << " : function(){" << "\n"; // triggered function start
        for(Input* input : node->inputs){
            if(input->port->isVacant()){ // unassigned inputs undefined
                logField->log(LogMessageTypes::warning, "Input " + input->name + " of " + *node->getName() + " unassigned.");
                writer << "\t\tvar " << input->name << " = undefined;" << "\n";
            }
            else{ // import others from respective nodes
                NodePort* inputPort = input->port;
                NodePort* outputPort = inputPort->getConnection()->other(inputPort);
                Node* otherNode = outputPort->getParent();
                std::string outputName = outputPort->output->name;
                writer << "\t\tvar " << input->name <<" = " << (nodeBasePrefix + nodeNumbering.at(otherNode) + nodeBaseSuffix) << "." << outputName << ";" << "\n";
                if(input->type != "any"){
                    if(input->type != outputPort->output->type){
                        logField->log(LogMessageTypes::warning, "Input " + input->name + " of " + *node->getName() + " of type " + input->type + " connected to output " + outputPort->output->name + " of " + *otherNode->getName() + " of type " + outputPort->output->type + ".");
                    }
                }
            }
        }
        for(Output* output : node->outputs){ // declare local output variables so they can be used in user code
            writer << "\t\tvar " << output->name << " = undefined;" << "\n";
        }
        std::string runScript = *(node->runScript);
        fillPlaceholders(runScript, node, nodeNumbering[node]);
        std::stringstream runScriptStream(runScript);
        std::string runScriptBuffer;
        while(getline(runScriptStream, runScriptBuffer)){ // now write actual script
            writer << "" << runScriptBuffer << "\n";
        }
        for(Output* output : node->outputs){ // and export outputs
            writer << "\t\tthis." << output->name << " = " << output->name << "\n";
        }
        for(NodePort* outTrigger : node->triggerOut){ // in the end sent out trigger signals
            if(!outTrigger->isVacant()){
                NodePort* inTrigger = outTrigger->getConnection()->other(outTrigger);
                writer << "\t\t" << (nodeBasePrefix + nodeNumbering.at(inTrigger->getParent()) + nodeBaseSuffix) << "." << nodeRunningFunction << "();" << "\n";
            }
        }
        writer << "\t}," << "\n"; // end of triggered function
        writer << "}" << "\n"; // end of object
        writer << (nodeBasePrefix + nodeNumbering.at(node) + nodeBaseSuffix) << "." << nodeInitFunction << "();" << "\n"; // call initializing function immediately afterwards
    }

    writer << "\"\"\"" << "\n" << "\n";

    writer << "feedback = \"\"\"" << "\n" << "feedback" << "\n" << "\"\"\"" << "\n" << "\n";

    logField->log(LogMessageTypes::success, "Task built and written successfully!");
}

void TaskAssembly::setGraphEditor(GraphEditor* graphEditor){
    TaskAssembly::graphEditor = graphEditor;
}

void TaskAssembly::setProjectProperties(ProjectProperties* projectProperties){
    TaskAssembly::projectProperties = projectProperties;
}

void TaskAssembly::setTaskPreview(SmallPreview* preview){
    TaskAssembly::preview = preview;
}

void TaskAssembly::setLogField(LogField* logField){
    TaskAssembly::logField = logField;
}

void TaskAssembly::fillPlaceholders(std::string& into, Node* from, std::string nodeNumber){
    replaceAllInString(into, "{HallGUIUID}", nodeUIDPrefix + nodeNumber + nodeUIDSuffix);
    replaceAllInString(into, "{HallGUIRunFunction}", nodeRunningFunction);
    replaceAllInString(into, "{HallGUIThisNode}", (nodeBasePrefix + nodeNumber + nodeBaseSuffix));
    for(Option* option : from->options){
        replaceAllInString(into, "{" + option->name + "}", option->core->getValue());
    }
}

void TaskAssembly::replaceAllInString(std::string& where, const std::string& what, const std::string& with){
    while(true){
        size_t start_pos = where.find(what);
        if(start_pos != std::string::npos){
            where.replace(start_pos, what.length(), with);
        }
        else{
            break;
        }
    }
}

bool TaskAssembly::checkCycleStep(std::vector<Node*>* nodes, std::unordered_map<Node*, int>* nodeNumbering, bool* checked, bool* onStack, int position){
    checked[position] = true;
    onStack[position] = true;
    for(NodePort* outTrigger : (*nodes)[position]->triggerOut){
        if(outTrigger->getConnection() != NULL){
            Node* other = outTrigger->getConnection()->other(outTrigger)->getParent();
            if(!checked[(*nodeNumbering)[other]]){
                if(checkCycleStep(nodes, nodeNumbering, checked, onStack, (*nodeNumbering)[other])){
                    return true;
                }
            }
            else if(onStack[(*nodeNumbering)[other]]){
                logField->log(LogMessageTypes::error, "An execution cycle was found containing the nodes " + *((*nodes)[position]->getName()) + " and " + *other->getName());
                return true;
            }
        }
    }
    onStack[position] = false;
    return false;
}

bool TaskAssembly::checkForCycle(){
    // via depth first search
    std::unordered_map<Node*, int> nodeNumbering;
    std::vector<Node*> nodes;
    int nodeNumber = 0;
    for(Node* node : graphEditor->nodes){
        nodes.push_back(node);
        nodeNumbering.insert(std::pair<Node*, int>(node, nodeNumber));
        nodeNumber++;
    }
    bool* checked = new bool[nodeNumber];
    bool* onStack = new bool[nodeNumber];
    for(int a = 0; a < nodeNumber; a++){
        checked[a] = false;
        onStack[a] = false;
    }
    bool found = false;
    for(int a = 0; a < nodeNumber; a++){
        if(!checked[a]){
            if(checkCycleStep(&nodes, &nodeNumbering, checked, onStack, a)){
                found = true;
            }
        }
    }
    delete checked;
    delete onStack;
    return found;
}
