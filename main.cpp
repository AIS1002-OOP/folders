#include <deque>
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class folder {

public:
    explicit folder(std::string name) : name_(std::move(name)) {}

    void add(folder &f) {
        f.parent_ = this;
        children_.push_back(&f);
    }

    bool remove(folder &f) {
        if (&f == this) {
            return false;
        }
        folder* found = nullptr;
        traverseDepthFirst([&](folder *ff) {
            if (ff == &f) {
                found = &f;
            }
        });
        if (found) {
            found->parent_ = nullptr;
            children_.erase(std::remove(children_.begin(), children_.end(), found), children_.end());
        }
        return found != nullptr;
    }

    [[nodiscard]] std::string name() const {
        return name_;
    }

    [[nodiscard]] std::string fullName() const {
        std::deque<std::string> deque;
        traverseUpwards([&deque](const folder *f) {
            deque.emplace_back(f->name_);
        });

        std::string result;
        while (!deque.empty()) {
            result += deque.back();
            deque.pop_back();
            if (!deque.empty()) {
                result += "\\";
            }
        }

        return result;
    }

    [[nodiscard]] bool hasParent() const {
        return parent_ != nullptr;
    }


private:
    std::string name_;

    folder *parent_ = nullptr;
    std::vector<folder *> children_;

    void traverseUpwards(const std::function<void(const folder *)> &f) const {
        f(this);
        if (hasParent()) {
            parent_->traverseUpwards(f);
        }
    }

    void traverseDepthFirst(const std::function<void(folder *)> &f) {
        f(this);
        for (auto c: children_) {
            c->traverseDepthFirst(f);
        }
    }
};

std::ostream &operator<<(std::ostream &os, const folder &f) {
    os << f.fullName();
    return os;
}

int main() {

    folder root("C:");
    folder bar("bar");
    folder baz("baz");

    root.add(bar);
    bar.add(baz);

    std::cout << baz << std::endl;

    root.remove(bar);

    std::cout << root << std::endl;

    std::cout << baz << std::endl;
    return 0;
}
