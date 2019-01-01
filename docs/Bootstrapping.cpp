// Detail Implementationen

class Drawable
{
	virtual draw(Window& window, Transform& transform = Transform::Identity); 
}

class Component : public Drawable, public Transformable
{
public:
	virtual void update(Orbis& orbis) { };
	
	virtual void draw(Window& window, Transform& transform) { };
}

class Shape : Component
{
public:
	void draw(Window& window, Transform& transform = Transform::Identity) 
	{
		transform *= getTransform();
		window.draw(mesh, transform);
	}	
	
private:
	Mesh m_mesh;
}	

class Level  
{
public:
	void updateLevel() {
		for_each(actor: m_actors)
			actor.update();
	}
	
	void drawLevel(Window& window) {
		for_each(actor: m_actors)
			actor.draw(window);
	}
	
private:
	std::vector<Actor*> m_actors;
}	

class Actor : public Transformable
{
public:
	void update() {
		for_each (component in m_components) 
			component.update(orbis);
		
		for_each(childActor in m_childActors)
			childActor.update();
	}			
	
	void draw(Window& window, Transform& transform = Transform::Identity)
	{
		transform *= getTransform;
		
		for_each(component in m_components)
			component.draw(window, transform);
			
		for_each(childActor in m_childActors)
			childActor.draw(window, transform);
	}
	
	void addComponent(Component& component);
	
	void addChild(Actor& actor);
	
private:
	std::vector<Component*> m_components;
	
	std::vector<Actor*> m_childActors;
}

class ShapeActor : Actor
{
	Drawable& getDrawable() { return m_shape; }
	
private:
	Shape m_shape;
}

class Scene
{
	typedef void (*SceneRunner)(Orbis&, Window&);
	
public:
	Scene(SceneRunner sceneRunner) 
		: m_runner(sceneRunner), m_isRunning(false) 
	{ }

	void add(Component& component);
	
	void add(Actor& actor);
	
	bool isRunning() const { return m_isRunning; }
	
	void isRunning(bool running) { m_isRunning = running; } 
	
	void update();
	
	run(Window& window) {
		m_runner(*this, window);
	}
	
private:
	std::vector<Actor&> m_actors;

	std::vector<Component&> m_components;
	
	SceneRunner m_runner;
	
	bool m_isRunning;
}

class Window
{
public:
	void setScene(Scene& scene) 
	{
		if (m_scene)
			m_scene->isRunning(false);		
		m_scene = &scene; 
		scene->isRunning(true);
	}
	
	void hasScene(){ return m_scene != NULL }
	
	void runScene() { scene->run(*this); }
	
private:
	Scene* m_scene;
}

// Bootstrapping Stufe 4: Levels
void example4a(Scene& scene, Window& window)
{	
	orb::Actor actor;
	actor.addComponent(Talker("Talking text 1"));
	actor.addComponent(Talker("Talking text 2"));
	scene.add(actor);
	
	Talker nakedComponent("Talking text from naked component without an actor");
	scene.add(nakedComponent);
	
	while(scene.isRunning()) {
		scene.update();
		window.setScene(Scene(example4b));
	}	
}

void example4b(Scene& scene, Window& window)
{
	Talker talker("Hello from scene 2");

	while(scene.isRunning()) {
		scene.update();
		window.close();
	}
}

void main()
{
	Window window;
	window.setScene(Scene(example4a));
	
	while(window.hasScene()) {
		window.runScene();
	}
}

// Bootstrapping Stufe 3: Actors mit Hierarchien
void example3()
{
	orb::Window window(400, 400, "My Title");

	orb::ShapeActor arm(4);
	arm.getDrawable().setPosition(10, 10);		
	arm.addComponent(Talker("Talking text"));					// this text is shown every frame
	person[0].position = orb::Vector2f(0, 0);
	person[1].position = orb::Vector2f(2, 0);
	person[2].position = orb::Vector2f(2, 0.5);	
	person[2].position = orb::Vector2f(0, 0.5);
	
	orb::ShapeActor person(4);
	person.addChild(arm);
	person[0].position = orb::Vector2f(0, 0);
	person[1].position = orb::Vector2f(1, 0);
	person[2].position = orb::Vector2f(1, 1);	
	person[2].position = orb::Vector2f(1, 0);	
	
	while (window.isOpen()){
		window.update();
		window.clear();
		person.update();
		person.draw(window);
		window.display();
	}
}

// Bootstrapping Stufe 2: Transformation für SFML-Style Entities
void example2()
{
	orb::Window window(400, 400, "My Title");
	
	orb::Shape triangle(3);
	triangle[0].position = orb::Vector2f(0, 0);
	triangle[1].position = orb::Vector2f(0, 1);
	triangle[2].position = orb::Vector2f(1, 0);
	triangle.setPosition(20, 30);
	
	while (window.isOpen()){
		window.update();
		window.clear();
		shape.draw(window);
		window.display();
	}
}

// Bootstrapping Stufe 1: SFML-Entities
void example1()
{
	orb::Window window(400, 400, "My Title");
	
	orb::Shape triangle(3);
	triangle[0].position = orb::Vector2f(0, 0);
	triangle[1].position = orb::Vector2f(0, 1);
	triangle[2].position = orb::Vector2f(1, 0);
	
	orb::Shape particles(3 /*number of vertices per particle*/, 2 /*number of particles*/)
	particles[0].position = orb::Vector2f(0, 0);
	particles[1].position = orb::Vector2f(0, 1);
	particles[2].position = orb::Vector2f(1, 0);
	particles[3].position = orb::Vector2f(1, 1);
	particles[4].position = orb::Vector2f(1, 2);
	particles[5].position = orb::Vector2f(2, 1);
	
	while (window.isOpen()){
		window.update();
		window.clear();
		shape.draw(window);
		shape.draw(particles);
		window.display();
	}
}

void main() 
{
	example1();
	example2();
	example3();
	example4();
}