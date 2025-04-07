#pragma once
#include <deque>
#include <vector>
#include <string>

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>
//#include <gtest/gtest.h>

// ------------------------------
// Абстрактный интерфейс (должен быть реализован)
// ------------------------------
class FileSystemComponent {
public:
    virtual ~FileSystemComponent() = default;
    virtual int get_size() const = 0; // TODO: реализовать в наследниках
    virtual void add_component(std::shared_ptr<FileSystemComponent> component) {} // Для папок
    virtual void remove_component(std::shared_ptr<FileSystemComponent> component) {} // Для папок
};

// ------------------------------
// Ваш код ниже (реализуйте недостающие части)
// ------------------------------

class File : public FileSystemComponent {
public:
    File(int size) : _size(size) {}

    int get_size() const override { 
        return _size;
    }

private:
    int _size;
};

class Folder : public FileSystemComponent {
public:

    int get_size() const override {
        return std::accumulate(
            _components.begin(), _components.end(), 
            0, 
            [](int sum, const auto& component) {
                return sum + component->get_size();
            }
        );
    }

    void add_component(std::shared_ptr<FileSystemComponent> component) override {
        _components.push_back(component);
    }

    void remove_component(std::shared_ptr<FileSystemComponent> component) override {
        _components.erase(std::remove(_components.begin(), _components.end(), component), _components.end());
    }

private:
    std::vector<std::shared_ptr<FileSystemComponent>> _components;
};
