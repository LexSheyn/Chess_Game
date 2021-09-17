#pragma once
class AttributeComponent
{
public:

//Variables:

	//Stats
	int hp;
	int hpMax;
	int damage;

//Constructors and Destructor:

	AttributeComponent(const int hp, const int damage);
	virtual ~AttributeComponent();

//Accessors:

	const bool isDead() const;

//Functions:

	void loseHp(const int hp);
	void gainHp(const int hp);

	void update();
};

