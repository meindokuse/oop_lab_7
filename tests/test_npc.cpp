#include <gtest/gtest.h>
#include "NPCFactory.h"
#include "Editor.h"

TEST(NPCTest, CreateKnight) {
    auto knight = NPCFactory::createNPC("Knight", "TestKnight", 100, 100);
    EXPECT_EQ(knight->getType(), "Knight");
    EXPECT_EQ(knight->getName(), "TestKnight");
    EXPECT_DOUBLE_EQ(knight->getX(), 100);
    EXPECT_DOUBLE_EQ(knight->getY(), 100);
}

TEST(NPCTest, InvalidCoordinates) {
    EXPECT_THROW(NPCFactory::createNPC("Knight", "Test", -10, 100), 
                 std::invalid_argument);
    EXPECT_THROW(NPCFactory::createNPC("Elf", "Test", 100, 600), 
                 std::invalid_argument);
}

TEST(NPCTest, Distance) {
    auto npc1 = NPCFactory::createNPC("Knight", "K1", 0, 0);
    auto npc2 = NPCFactory::createNPC("Elf", "E1", 3, 4);
    EXPECT_DOUBLE_EQ(npc1->distanceTo(*npc2), 5.0);
}

TEST(EditorTest, AddAndCount) {
    Editor editor;
    editor.addNPC(NPCFactory::createNPC("Dragon", "D1", 100, 100));
    editor.addNPC(NPCFactory::createNPC("Knight", "K1", 200, 200));
    EXPECT_EQ(editor.getNPCCount(), 2);
}

TEST(EditorTest, SaveAndLoad) {
    Editor editor;
    editor.addNPC(NPCFactory::createNPC("Knight", "TestK", 150, 150));
    editor.saveToFile("test_save.txt");
    
    Editor editor2;
    editor2.loadFromFile("test_save.txt");
    EXPECT_EQ(editor2.getNPCCount(), 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
