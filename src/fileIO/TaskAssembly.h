#ifndef TASKASSEMBLY_H_INCLUDED
#define TASKASSEMBLY_H_INCLUDED

#include <string>
#include <vector>
#include <unordered_map>

////FORWARD DECLARATIONS
class GraphEditor;
class ProjectProperties;
class LogField;
class SmallPreview;
class Node;
////

/** \brief A class with only static contents meant for the writing of the output script from the current project state.
 */
class TaskAssembly{
private:
    static GraphEditor* graphEditor;/**<The graph editor which contains the nodes of the project.*/
    static ProjectProperties* projectProperties;/**<The gui element for all the global properties of the project.*/
    static SmallPreview* preview;/**<The task preview which decides the positioning of the task parts of the nodes.*/
    static LogField* logField;/**<The log field where messages relating to building the project will be placed.*/
public:
    /** \brief Assembles the output scripts and writes it to the hard disk in the running directory as result.py.
     *
     * \return void
     *
     */
    static void assemble();
    /** \brief Sets a GraphEditor to read the nodes and connections from.
     *
     * \param graphEditor GraphEditor* The GraphEditor used for this project.
     * \return void
     *
     */
    static void setGraphEditor(GraphEditor* graphEditor);
    /** \brief  Sets a ProjectProperties to read all global properties of this project from.
     *
     * \param projectProperties ProjectProperties* The ProjectProperties used for this project.
     * \return void
     *
     */
    static void setProjectProperties(ProjectProperties* projectProperties);
    /** \brief  Sets a task preview to get the order of the nodes in the task from.
     *
     * \param preview SmallPreview* The task preview to get the order of the nodes in the task from.
     * \return void
     *
     */
    static void setTaskPreview(SmallPreview* preview);
    /** \brief  Sets a log field to send build messages to.
     *
     * \param preview SmallPreview* The log field where the build messages will be shown.
     * \return void
     *
     */
    static void setLogField(LogField* logField);
private:
    /** \brief Fills all placeholders for local and global options.
     *
     * \param into std::string& The string where placeholders are to be replaced.
     * \param from Node* The node this string belongs to.
     * \param nodeNumber std::string The script-internal numbering of the Node from.
     * \return void
     *
     */
    static void fillPlaceholders(std::string& into, Node* from, std::string nodeNumber);
    /** \brief Replaces all occurrences of a substring in a string with another string.
     *
     * \param where std::string& The string where parts are to be replaced.
     * \param what const std::string& The string to be replaced.
     * \param with const std::string& The string to replace with.
     * \return void
     *
     */
    static void replaceAllInString(std::string& where, const std::string& what, const std::string& with);
    /** \brief A recursive function used by checkForCycle to find cycles in the graph. Logs found cycles by giving two nodes inside it. Starts a depth first search from the given position.
     *
     * \param nodes std::vector<Node*>* A vector containing all the nodes.
     * \param nodeNumbering std::unordered_map<Node*, int> A mapping, which maps nodes to their positions in the vector nodes.
     * \param checked bool* Whether any node has already been searched according to the given numbering.
     * \param onStack bool* Whether any node is currently on the recursion stack according to the given numbering.
     * \param position int The position of the node to search currently.
     * \return bool True exactly if a cycle was found.
     *
     */
    static bool checkCycleStep(std::vector<Node*>* nodes, std::unordered_map<Node*, int>* nodeNumbering, bool* checked, bool* onStack, int position);
    /** \brief Checks whether the graph contains any cycles. Also causes logging of two nodes for every found cycle.
     *
     * \return bool True exactly if there is a cycle in the graph.
     *
     */
    static bool checkForCycle();
};

#endif // TASKASSEMBLY_H_INCLUDED
