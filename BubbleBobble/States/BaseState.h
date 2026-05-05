class BaseState
{
	public:
	virtual ~BaseState() = default;
	virtual void Enter() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void HandleInput() = 0;
	virtual void Exit() = 0;
};