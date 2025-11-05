#include "xmlnode.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>

XmlNode::XmlNode(std::string name) : m_name(std::move(name)) {}

XmlNode::~XmlNode()
{
    m_parent.reset();
    std::cout << "destroying " << m_name << "\n";
}

void XmlNode::addChild(const std::string& name)
{
    auto child = create(name);
    addChild(child);
}

void XmlNode::addChild(XmlNode::Ptr child)
{
    // check if unique name
    if (nullptr != getChild(child->getName()))
    {
        throw std::runtime_error("Child already exists in this node.");
    }
    if (isAncestor(child))
    {
        throw std::runtime_error(
            "Cannot add ancestor as a child, you pervert.");
    }

    // set myself as a weak parent ptr in the child
    child->m_parent = weak_from_this();
    m_children.push_back(std::move(child));
}

bool XmlNode::deleteChild(const std::string& name)
{
    auto child = getChild(name);
    if (child != nullptr)
    {
        return deleteChild(child);
    }
    return false;
}

bool XmlNode::deleteChild(const XmlNode::Ptr& childToDelete)
{
    auto child = std::find(m_children.begin(), m_children.end(), childToDelete);
    if (child != m_children.end())
    {
        // remove reference to myself
        (*child)->m_parent.reset();
        m_children.erase(child);
        return true;
    }
    return false;
}

XmlNode::Ptr XmlNode::getChild(const std::string& name) const
{
    auto found = std::ranges::find_if(m_children, [&name](const auto& child) {
        return child->getName() == name;
    });
    return found != m_children.end() ? *found : nullptr;
}

bool XmlNode::isAncestor(const XmlNode::Ptr& node) const
{
    auto current = getParent();
    while (current)
    {
        if (current == node)
        {
            return true;
        }
        current = current->getParent();
    }
    return false;
}
