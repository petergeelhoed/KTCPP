#include "xmlnode.hpp"

#include <iostream>

int main()
{
    auto root = std::make_shared<XmlNode>("root");
    auto child2 = std::make_shared<XmlNode>("child2");
    auto child3 = std::make_shared<XmlNode>("child3");

    {
        auto child1 = std::make_shared<XmlNode>("child1");
        root->addChild(child1);
        root->addChild(child2);
        root->addChild(child3);

        std::cout << "Children before deletion:\n";
        for (const auto& child : root->getChildren())
        {
            std::cout << "- " << child->getName() << "\n";
        }
        auto favourite = root->getChild("child1");
        if (favourite)
        {
            std::cout << "favourite: " << favourite->getName() << "\n";
        }

        root->deleteChild(child1);

        std::cout << child1->getName() << " still exists\n";
    } // scope for child1
    std::cout << "Children after deletion:\n";
    for (const auto& child : root->getChildren())
    {
        std::cout << "- " << child->getName() << "\n";
    }

    // test unique childnames
    std::cout << "Test unique Children(cannot add child2):\n";
    try
    {
        root->addChild(child2);
    }
    catch (const std::runtime_error& e)
    {
        std::cout << " " << e.what() << "\n";
    }

    std::cout << "Test unknown child:\n";
    auto cuckoo = root->getChild("other");
    if (cuckoo != nullptr)
    {
        std::cout << " Cuckoo found, eject from nest\n";
    }
    else
    {
        std::cout << " All mine\n";
    }

    // what happens here?
    // child3 = child2;
    // child3 refcount will go down. but not destroyed yet as
    // it is in the root

    // but this does not compile because we have no copy
    // use of deleted function ‘XmlNode& XmlNode::operator=(const XmlNode&)’
    // XmlNode bastard1("bastard1");
    // XmlNode bastard2("bastard1");
    //    bastard1 = bastard2;

    // this does compile if we have move semantics
    //    bastard1 = std::move(bastard2);

    // but you cannot add it to the root
    // as it should be on the heap as a shared pointer
    //    root->addChild(&bastard1);

    // so why do I need the move of the object?
    //

    return 0;
}
