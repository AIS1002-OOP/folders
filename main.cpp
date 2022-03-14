
#include <iostream>
#include <functional>
#include <utility>
#include <vector>
#include <string>

void loop(int i, int count, const std::function<void()>& f) {
    if (i < count) {
        f();
        loop(++i, count, f);
    }
}


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

    void traverseUpwards(const std::function<void(folder*)>& f) {
        f(this);
        if (parent_ != nullptr) {
            parent_->traverseUpwards(f);
        }
    }

private:
    std::string name_;

    folder* parent_ = nullptr;
    std::vector<folder*> children_;

};


int main() {

    folder c("C:");
    folder bar("bar");
    folder baz("baz");

    c.add(bar);
    bar.add(baz);

    baz.traverseUpwards([](folder* f){
        std::cout << f->getName() << std::endl;
    });

    return 0;
}
