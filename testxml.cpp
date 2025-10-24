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

    return 0;
}
