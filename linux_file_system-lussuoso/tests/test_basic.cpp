#include <gtest/gtest.h>
#include "library.hpp"

#include <gtest/gtest.h>

TEST(CompositeTest, FileReturnsCorrectSize) {
    auto file = std::make_shared<File>(100);
    ASSERT_EQ(file->get_size(), 100); // Тест упадет, пока get_size() не реализован
}

TEST(CompositeTest, FolderCalculatesTotalSize) {
    auto root = std::make_shared<Folder>();
    auto subFolder = std::make_shared<Folder>();
    
    subFolder->add_component(std::make_shared<File>(200));
    subFolder->add_component(std::make_shared<File>(300));
    root->add_component(subFolder);
    root->add_component(std::make_shared<File>(500));

    ASSERT_EQ(root->get_size(), 200 + 300 + 500); // Тест упадет, пока логика не реализована
}

TEST(CompositeTest, DynamicModification) {
    auto root = std::make_shared<Folder>();
    auto file = std::make_shared<File>(1000);
    
    root->add_component(file);
    ASSERT_EQ(root->get_size(), 1000); 

    root->remove_component(file);
    ASSERT_EQ(root->get_size(), 0); // Тест упадет, пока remove_component() не работает
}

TEST(CompositeTest, NestedFolders) {
    auto root = std::make_shared<Folder>();
    auto folder1 = std::make_shared<Folder>();
    auto folder2 = std::make_shared<Folder>();

    folder1->add_component(std::make_shared<File>(10));
    folder2->add_component(std::make_shared<File>(20));
    folder1->add_component(folder2);
    root->add_component(folder1);

    ASSERT_EQ(root->get_size(), 10 + 20); // Проверка вложенности
}
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}