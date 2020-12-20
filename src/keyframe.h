class KeyFrame {
public:
	int frame = -1;     //  -1 => no key is set;
	glm::vec3 position = glm::vec3(0, 0, 0);   // translate channel
	glm::vec3 rotation = glm::vec3(0, 0, 0);   // rotate channel
	SceneObject *obj = NULL;                   // object that is keyframed
};