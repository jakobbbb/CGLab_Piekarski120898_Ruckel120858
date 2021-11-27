#include <SceneGraph.hpp>

/* constructor for SceneGraph */
SceneGraph::SceneGraph()
    : root_{std::make_shared<Node>(Node{nullptr, "root"})} {}

/* return the name of the scene graph */
std::string SceneGraph::getName() const {
    return name_;
}

/* print the scene graph */
std::string SceneGraph::printGraph() const {
    std::string graph = "Graph: ";
    for (auto const& node : root_->getChildrenList()) {
        graph.append(node->getName());
        graph.append(", ");
    }
    return graph;
}

/* return pointer to the root node of the scene graph */
std::shared_ptr<Node> SceneGraph::getRoot() const {
    return root_;
}

/* set the root of the scene graph */
void SceneGraph::setRoot(std::shared_ptr<Node> const& root) {
    root_ = root;
}

/* set a name for the scene graph */
void SceneGraph::setName(std::string const& name) {
    name_ = name;
}

/* print the scene graph */
std::ostream& SceneGraph::print(std::ostream& os) const {
    root_->print(os);
    return os;
}

/* store traverse function for root node */
void SceneGraph::traverse(node_traverse_func func) {
    func(root_);
    root_->traverse(func);
}

/* return an active camera */
std::shared_ptr<CameraNode> SceneGraph::getActiveCamera() {
    std::shared_ptr<CameraNode> camera;
    node_traverse_func find_camera = [&](std::shared_ptr<Node> node) {
        auto cam_node = std::dynamic_pointer_cast<CameraNode>(node);
        if (cam_node && cam_node->getEnabled()) {
            camera = cam_node;
        }
    };
    getInstance().traverse(find_camera);
    return camera;
}

std::shared_ptr<PointLightNode> SceneGraph::getLight() {
    std::shared_ptr<PointLightNode> light;
    node_traverse_func find_light = [&](std::shared_ptr<Node> node) {
        auto light_node = std::dynamic_pointer_cast<PointLightNode>(node);
        if (light_node) {
            light = light_node;
            return;
        }
    };
    getInstance().traverse(find_light);
    return light;
}

std::shared_ptr<Node> SceneGraph::find(std::string const& name) {
    return getInstance().getRoot()->getChildren(name);
}

/* destructor for SceneGraph */
SceneGraph::~SceneGraph() {}

SceneGraph& SceneGraph::getInstance() {
    static SceneGraph instance;
    return instance;
}

/* output operator */
std::ostream& operator<<(std::ostream& os, SceneGraph const& s) {
    return s.print(os);
}
