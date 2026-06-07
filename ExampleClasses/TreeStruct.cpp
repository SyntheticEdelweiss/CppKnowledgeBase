#include "TreeStruct.hpp"

using namespace std;
using Id_t = TreeStruct::Id_t;
using Flat = TreeStruct::Flat;

QDataStream& operator<<(QDataStream& stream, const Flat& data)
{
    stream << data.d.id;
    stream << data.d.name;
    stream << data.parent_id;
    stream << data.children_id;
    return stream;
}
QDataStream& operator>>(QDataStream& stream, Flat& data)
{
    stream >> data.d.id;
    stream >> data.d.name;
    stream >> data.parent_id;
    stream >> data.children_id;
    return stream;
}

std::shared_ptr<TreeStruct> TreeStruct::getRoot()
{
    auto result = shared_from_this();
    while (!result->parent.expired())
        result = parent.lock();
    return result;
}
std::shared_ptr<TreeStruct> TreeStruct::findById(Id_t id)
{
    if (d.id == id)
        return shared_from_this();
    for (auto& e : children)
    {
        auto childResult = e->findById(id);
        if (childResult)
            return childResult;
    }
    return {};
}
std::shared_ptr<TreeStruct> TreeStruct::findByIdFromRoot(Id_t id) { return getRoot()->findById(id); }

QDataStream& operator<<(QDataStream& stream, const TreeStruct& data)
{
    auto flatList = data.toFlatList();
    stream << flatList;
    return stream;
}
QDataStream& operator>>(QDataStream& stream, TreeStruct& data)
{
    QList<TreeStruct> flatList;
    stream >> flatList;
    return stream;
}
void TreeStruct::fromJsonObject(QJsonObject const& obj)
{

}
QJsonObject TreeStruct::toJsonObject() const
{
    QJsonObject result;
    return result;
}

QList<Flat> TreeStruct::toFlatList() const
{
    QList<Flat> result;
    result.append(Flat(*this));
    for (const auto& e : this->children)
        result.append(e->toFlatList());
    return result;

}
void TreeStruct::fromFlatList(const QList<Flat>& flatList, Id_t this_id)
{
    QMap<Id_t, Flat> flatMap;
    for (const auto& e : flatList)
        flatMap.insert(e.d.id, e);
    fromFlatMap(flatMap, this_id);
}
QMap<Id_t, Flat> TreeStruct::toFlatMap() const
{
    QMap<Id_t, Flat> result;
    result.insert(this->d.id, Flat(*this));
    for (const auto& e : this->children)
        result.insert(e->toFlatMap());
    return result;
}
void TreeStruct::fromFlatMap(const QMap<Id_t, Flat>& flatMap, Id_t this_id)
{
    const Flat* this_flat;
    if (this_id == 0)
    {
        const auto iter = std::find_if(flatMap.cbegin(), flatMap.cend(), [](auto& e) { return (e.parent_id == 0); });
        assert(iter != flatMap.cend());
        this->d = iter->d;
        this_flat = &*iter;
    }
    else
    {
        const auto iter = flatMap.constFind(this_id);
        assert(iter != flatMap.cend());
        this->d = iter->d;
        this_flat = &*iter;
    }
    this->children.clear();
    for (const auto& child_id : this_flat->children_id)
    {
        auto child = make_shared<TreeStruct>();
        child->parent = shared_from_this();
        child->fromFlatMap(flatMap, child_id);
        this->children.append(child);
    }
}
