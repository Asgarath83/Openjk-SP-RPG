// bg_ammo.h
// Contains data for the ammo.json table
// (c) 2013 Jedi Knight Galaxies

#pragma once
#include "../qcommon/q_shared.h"

#define MAX_AMMO_NAME	32
#define MAX_AMMO_TYPES	256

template <typename T>
struct complexAmmoOverride {
	qboolean bIsPresent;
	qboolean bSet;
	qboolean bAdd;
	qboolean bMultiply;

	T set;
	T add;
	float multiply;
};

struct NewAmmo_t 
{
	char name[MAX_AMMO_NAME];
	char shortname[MAX_AMMO_NAME];
	char substitute[MAX_AMMO_NAME];
	int ammoIndex;
	int ammoMax;
	float pricePerUnit;

	NewAmmo_t* pSub;	// pointer to the substitute..may point to itself!!

	struct 
	{
		// Should correspond to the ammoOverrideTypes_t enum
		std::pair<qboolean, int> means;
		std::pair<qboolean, int> splashmeans;

		complexAmmoOverride<int> damage;
		complexAmmoOverride<int> projectiles;
		complexAmmoOverride<double> splashRange;
		complexAmmoOverride<double> collisionSize;
		complexAmmoOverride<double> recoil;
		complexAmmoOverride<int> ammocost;
		complexAmmoOverride<int> fireDelay;
		complexAmmoOverride<int> bounces;
		complexAmmoOverride<int> accuracyRatingBase;
		complexAmmoOverride<int> accuracyRatingPerShot;
		complexAmmoOverride<double> knockback;
		complexAmmoOverride<double> speed;

		std::pair<qboolean, int> useGravity;


		std::pair<qboolean, int> hitscan;

	} overrides;

	// visual overrides, only on the client
	struct {
		std::pair<qboolean, std::string> crosshairShader;

		// "fire"
		complexAmmoOverride<double> muzzleLightIntensity;
		std::pair<qboolean, vec3_t> muzzleLightColor;
		std::pair<qboolean, std::string> chargingEffect;
		std::pair<qboolean, std::string> muzzleEffect;
		std::pair<qboolean, std::vector<std::string>> fireSound;

		struct {
			std::pair<qboolean, std::string> shader;
			complexAmmoOverride<double> minSize;
			complexAmmoOverride<double> maxSize;
			complexAmmoOverride<int> lifeTime;
		} traceline;

		struct {
			std::pair<qboolean, std::string> projectileModel;
			std::pair<qboolean, std::string> projectileEffect;
			std::pair<qboolean, std::string> runSound;

			complexAmmoOverride<double> lightIntensity;
			std::pair<qboolean, vec3_t> lightColor;

			std::pair<qboolean, std::string> deathEffect;
			std::pair<qboolean, std::string> impactEffect;
			std::pair<qboolean, std::string> deflectEffect;
		} projectile;

	} visualOverrides;
};

void InitializeAmmo ( void );
NewAmmo_t *GetAmmo ( const char *ammoName );
NewAmmo_t *GetAmmo ( const int ammoIndex );
void GetAllAmmoSubstitutions(int ammoIndex, std::vector<NewAmmo_t*>& outSubs);
bool WeaponAcceptsAlternateAmmo(int weapon, int variation);
qboolean AmmoIsBasedOn(int ammoTypeIndex, int basedOnIndex);

void ApplyAmmoOverride(int& value, const complexAmmoOverride<int>& field);
void ApplyAmmoOverride(double& value, const complexAmmoOverride<double>& field);
void ApplySimpleAmmoOverride(int& value, const std::pair<qboolean, int>& field);
void ApplySimpleAmmoOverride(double& value, const std::pair<qboolean, double>& field);
/*qboolean SimpleAmmoOverridePresent(const std::pair<qboolean, int>& field);
qboolean SimpleAmmoOverridePresent(const std::pair<qboolean, double>& field);
qboolean SimpleAmmoOverridePresent(const std::pair<qboolean, std::string>& field);
qboolean SimpleAmmoOverridePresent(const std::pair<qboolean, std::vector<std::string>>& field);
qboolean SimpleAmmoOverridePresent(const std::pair<qboolean, vec3_t>& field);*/

extern NewAmmo_t ammoTable[MAX_AMMO_TYPES];
extern int numAmmoLoaded;


/*std::pair<qboolean, vec3_t> muzzleLightColor;
        std::pair<qboolean, std::string> chargingEffect;
        std::pair<qboolean, std::string> muzzleEffect;
        std::pair<qboolean, std::vector<std::string>> fireSound;*/


struct fireSound 
{
    qboolean firesound_true;
    vec3_t firesound_vec;
};
/*
Oggi alle 19:38
this kind of estuff expeciasllu the std string
eezstreetOggi alle 19:38
Pair is just that, a pair
it has two different values
Oggi alle 19:38
oooh
like int a int b?
float a int c
etc?

ggi alle 19:39
yeah
you can specify the types
in this case, i use qboolean to specify "does this exist?"
and a second value to be "ok, if true, then use the second value"
very simple setup there
std::string is just a string
obviously
std::string is a lot easier in some ways than using const char*
or char*
Oggi alle 19:40
mmm
can you make a me a praticla example converting one of these std pair functions into the readable syntax for vs2010?

std::pair<qboolean, std::string> chargingEffect; for example that
how should become?

Oggi alle 19:41
because for example you can do this:
std::string myString;

myString = "hello " + "world!";

the alternative using C style strings is way more complicated:
char myBuffer[32];

strncpy(myBuffer, "hello ", 32);
strncat(myBuffer, "world!", 32);

well you can use the Pair that i defined above
and then just change from std::pair to Pair
but another way would be to break it apart into spearate things
qboolean hasChargingEffect;
std::string chargingEffectOverride;
*/