#include <algorithm>
#include <iostream>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>

class XmlNode : public std::enable_shared_from_this<XmlNode>
{
    using Ptr = std::shared_ptr<XmlNode>;

  public:
    // never use this!
    // use XmlNode::create to always make it with make_shared
    // somehow I cannot make this private
    //  With GCC/libstdc++, std::make_shared<T>(...) requires T's constructor to
    //  be public.
    explicit XmlNode(std::string name);

    // Factory: static member, calls constructor
    static Ptr create(const std::string& name)
    {
        return std::make_shared<XmlNode>(name);
    }

    ~XmlNode();
    // should be non copyable and non movable
    XmlNode(const XmlNode& other) = delete;
    XmlNode& operator=(const XmlNode& rhs) = delete;
    XmlNode(XmlNode&& other) noexcept = delete;
    XmlNode& operator=(XmlNode&& other) noexcept = delete;

    void addChild(const std::string& name);

    void addChild(Ptr child);

    bool deleteChild(const std::string& name);

    bool deleteChild(const Ptr& childToDelete);

    Ptr getChild(const std::string& name) const;

    Ptr getParent() const { return m_parent.lock(); }

    const std::vector<Ptr>& getChildren() const { return m_children; }

    const std::string& getName() const { return m_name; }

  protected:
    bool isAncestor(const Ptr& node) const;

  private:
    std::string m_name;
    std::weak_ptr<XmlNode> m_parent;
    std::vector<Ptr> m_children;
};
