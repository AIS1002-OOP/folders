
#include <iostream>
#include <functional>
#include <utility>
#include <vector>
#include <string>

class folder {

public:
    explicit folder(std::string name) : name_(std::move(name)) {}

    [[nodiscard]] std::string getName() const {
        return name_;
    }

    void add(folder& f) {
        f.parent_ = this; // this -> folder*
        children_.emplace_back(&f);
    }

    [[nodiscard]] bool isRoot() const {
        return parent_ == nullptr;
    }

    [[nodiscard]] bool isLeaf() const {
        return children_.empty();
    }

    [[nodiscard]] bool hasParent() const {
        return parent_ != nullptr;
    }

    void traverseUpwards(const std::function<void(folder*)>& f) {
        f(this);
        if (hasParent()) {
            parent_->traverseUpwards(f);
        }
    }

    void traverseDepthFirst(const std::function<void(folder*)>& f) {
        f(this);
        for (auto c : children_) {
            c->traverseDepthFirst(f);
        }
    }

private:
    std::string name_;

    folder* parent_ = nullptr;
    std::vector<folder*> children_;

};

int main() {

    folder c("C:");
    folder left("left");
    folder bar("bar");
    folder baz("baz");
    folder fubar("fubar");

    c.add(left);

    c.add(bar);
    bar.add(baz);
    bar.add(fubar);

    folder f1("f1");
    folder f2("f2");
    folder f3("f3");

    left.add(f1);
    left.add(f2);
    f2.add(f3);

    baz.traverseUpwards([](folder* f){
        std::cout << f->getName() << std::endl;
    });

    std::vector<folder*> collect;
    c.traverseDepthFirst([&](folder* f){
        collect.emplace_back(f);
    });

    return 0;
}
