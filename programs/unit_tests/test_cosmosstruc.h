#ifndef __TEST_COSMOSSTRUC
#define __TEST_COSMOSSTRUC

#include "support/configCosmos.h"
#include "support/jsonlib.h"
#include "gtest/gtest.h"

TEST(cosmosstruc, allocation)   {
	//cosmosstruc* c = new cosmosstruc();	
	cosmosstruc* c = json_init();	
	cout<<"\t\t\t\t\t\t\t\tcinfo = <"<<c<<">"<<endl;
	EXPECT_NE(c, nullptr);
}


// check every initialization value

// double timestamp
TEST(cosmosstruc, json_init_timestamp)	{

	cosmosstruc* c = json_init();
	double timestamp = c->timestamp;
	double current = currentmjd();
	cout<<"\t\t\t\t\t\t\t\ttimestamp = "<<timestamp<<"\tcurrent = "<<current<<endl;
	stringstream tmp;
	tmp << setprecision(6) << fixed << timestamp;
	timestamp = stod(tmp.str());
	tmp.str(string());
	tmp << setprecision(6) << fixed << current;
	current = stod(tmp.str());
	EXPECT_DOUBLE_EQ(timestamp, current);
}

// uint16_t jmapped
TEST(cosmosstruc, json_init_jmapped)	{
	cosmosstruc* c = json_init();
	// should equal total number of objects in the map
	EXPECT_EQ(c->jmapped, JSON_UNIT_COUNT + JSON_MAX_HASH );
}

// skip jmap
// skip emap

// vector<vector<unitstruc> unit
TEST(cosmosstruc, json_init_unit)	{
	cosmosstruc* c = json_init();

	// check size of unit vector (i.e. number of basic units)
	EXPECT_EQ(c->unit.size(), JSON_UNIT_COUNT);

	// check a random element

	// check there are 5 units of time (sec, min, hour, day, mjd)
	EXPECT_EQ(c->unit[JSON_UNIT_TIME].size(), 5);

	// check the settings of the 'day' time element
	EXPECT_EQ(c->unit[JSON_UNIT_TIME][3].name, "day");
	EXPECT_EQ(c->unit[JSON_UNIT_TIME][3].type, JSON_UNIT_TYPE_POLY);
	EXPECT_FLOAT_EQ(c->unit[JSON_UNIT_TIME][3].p0, 0.0);
	EXPECT_FLOAT_EQ(c->unit[JSON_UNIT_TIME][3].p1, 1.f/86400.f); 
	// this is actually prolly the wrong 2nd derivative ;)
	EXPECT_FLOAT_EQ(c->unit[JSON_UNIT_TIME][3].p2, 0.0);
}


// node info
TEST(cosmosstruc, json_init_node)	{
	cosmosstruc* c = json_init();

	EXPECT_EQ(string(c->node.name), "");
	EXPECT_EQ(c->node. vertex_cnt , 0);
    EXPECT_EQ(c->node. normal_cnt , 0);
    EXPECT_EQ(c->node. face_cnt , 0);
    EXPECT_EQ(c->node. piece_cnt , 0);
    EXPECT_EQ(c->node. device_cnt , 0);
    EXPECT_EQ(c->node. port_cnt , 0);
    EXPECT_EQ(c->node. agent_cnt , 0);
    EXPECT_EQ(c->node. event_cnt , 0);
    EXPECT_EQ(c->node. target_cnt , 0);
    EXPECT_EQ(c->node. user_cnt , 0);
    EXPECT_EQ(c->node. glossary_cnt , 0);
    EXPECT_EQ(c->node. tle_cnt , 0);
}



// check all get_value methods
TEST(cosmosstruc, get_value)	{
	cosmosstruc* c = json_init();
	c->add_name("UTC", &c->node.loc.utc);
	EXPECT_DOUBLE_EQ(c->get_value<double>("UTC"), 0.0);
}



// check all set_value methods
TEST(cosmosstruc, set_value)	{
	cosmosstruc* c = json_init();
	c->add_name("UTC", &c->node.loc.utc);
	c->set_value<double>("UTC", 5.432);
	EXPECT_DOUBLE_EQ(c->get_value<double>("UTC"), 5.432);
}

#endif
