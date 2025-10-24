
#include <algorithm>
#include <iostream>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>

class XmlNode : public std::enable_shared_from_this<XmlNode>
{
  public:
    using Ptr = std::shared_ptr<XmlNode>;

    explicit XmlNode(std::string name) : m_name(std::move(name)) {}

    ~XmlNode()
    {
        m_parent.reset();
        std::cout << "destroying " << m_name << "\n";
    };

    // should be non copyable and non movable
    XmlNode(const XmlNode& other) = delete;
    XmlNode& operator=(const XmlNode& rhs) = delete;
    XmlNode(XmlNode&& other) noexcept = delete;
    XmlNode& operator=(XmlNode&& other) noexcept = delete;

    void addChild(Ptr child)
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
        child->m_parent = shared_from_this();
        m_children.push_back(std::move(child));
    }

    bool deleteChild(const Ptr& childToDelete)
    {
        auto child =
            std::find(m_children.begin(), m_children.end(), childToDelete);
        if (child != m_children.end())
        {
            // remove reference to myself
            (*child)->m_parent.reset();
            m_children.erase(child);
            return true;
        }
        return false;
    }

    Ptr getChild(const std::string& name) const
    {
        auto found =
            std::ranges::find_if(m_children, [&name](const auto& child) {
                return child->getName() == name;
            });

        return found != m_children.end() ? *found : nullptr;
    }

    Ptr getParent() const { return m_parent.lock(); }

    const std::vector<Ptr>& getChildren() const { return m_children; }

    const std::string& getName() const { return m_name; }

    bool isAncestor(const Ptr& node) const
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

  private:
    std::string m_name;
    std::weak_ptr<XmlNode> m_parent;
    std::vector<Ptr> m_children;
};
