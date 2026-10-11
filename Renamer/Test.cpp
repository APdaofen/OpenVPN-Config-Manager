#include	<gtest/gtest.h>
#include	<filesystem>
#include	<chrono>
#include	"lib.h"
#ifndef		OVPNS
#define		OVPNS	""
#endif
#ifndef		OVPNS_AUTHED
#define		OVPNS_AUTHED	""
#endif
using namespace std;
using namespace chrono;
using namespace filesystem;

vector<string> new_templates = 
{
	"UnitedStates#1-UDP.ovpn", "UnitedStates#1-udp-authed.ovpn",
	"UnitedKingdom#2-UDP.ovpn", "UnitedKingdom#2-udp-authed.ovpn",
	"Canada#3-TCP.ovpn", "Canada#3-tcp-authed.ovpn",
	"Japan#4-TCP.ovpn", "Japan#4-tcp-authed.ovpn"
};

class Feature : public testing::Test
{
protected:
	db minthr = 100;
	db maxthr = 600;
	steady_clock::time_point birth;
	void SetUp (void) override
	{
		birth = steady_clock::now();
		system ("rmdir Templates /s /q");
		system ("xcopy ..\\..\\Templates Templates /I");
		system ("copy Template.ini Templates /Y");
	}
	void TearDown (void) override
	{
		// Old files
		for (auto i : templates)
		{
			EXPECT_FALSE (exists ("Templates\\" + i + ".ovpn"));
			EXPECT_FALSE (exists ("Templates\\" + i + "-authed.ovpn"));
		}
		// New files
		for (auto i : new_templates)
			EXPECT_TRUE (exists ("Templates\\" + i));
		db dt = duration_cast<duration<db,milli> > (steady_clock::now() - birth).count();
		EXPECT_LE (minthr, dt);
		EXPECT_LE (dt, maxthr);
	}
	void SetThr (db min, db max)
	{
		minthr = min;
		maxthr = max;
	}
};

TEST (Help, Help)
{
	EXPECT_EQ (0, system ("Renamer > stdout"));
	EXPECT_EQ (0, system ("fc stdout Help.txt /W"));
	EXPECT_EQ (0, system ("Renamer --help > stdout"));
	EXPECT_EQ (0, system ("fc stdout Help.txt /W"));
	EXPECT_EQ (0, system ("Renamer /help > stdout"));
	EXPECT_EQ (0, system ("fc stdout Help.txt /W"));
	EXPECT_EQ (0, system ("Renamer /? > stdout"));
	EXPECT_EQ (0, system ("fc stdout Help.txt /W"));
}

TEST_F (Feature, Basic) { EXPECT_EQ (0, system ("cd Templates && ..\\Renamer " OVPNS " " OVPNS_AUTHED " Template.ini")); }
TEST_F (Feature, Wildcard) { EXPECT_EQ (0, system ("Renamer Templates\\*")); }
