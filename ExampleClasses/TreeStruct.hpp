#pragma once

#include <memory>

#include <QtCore/QDataStream>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>

class TreeStruct : public std::enable_shared_from_this<TreeStruct>
{
public:
    using Id_t = quint32;
    struct Data
    {
        Id_t id = 0;
        QString name;
    };

    Data d;
    std::weak_ptr<TreeStruct> parent;
    QList<std::shared_ptr<TreeStruct>> children;

    std::shared_ptr<TreeStruct> getRoot();
    std::shared_ptr<TreeStruct> findById(Id_t id);
    std::shared_ptr<TreeStruct> findByIdFromRoot(Id_t id);

    friend QDataStream& operator<<(QDataStream& stream, const TreeStruct& data);
    friend QDataStream& operator>>(QDataStream& stream, TreeStruct& data);
    void fromJsonObject(QJsonObject const& obj);
    QJsonObject toJsonObject() const;

    /* Would be nice to make class Flat private inside TreeStruct, but it has some bullshit issues with type access rules.
     * You can declare friend function inside Flat with Flat type arg, but can't define it in cpp because that's type access violation,
     * because friend declaration grants access to private members of class, while Flat itself is private member of TreeStruct.
     * So you need to either leave this class public, or define respective functions inline in header (thus Flat type is unqualified and looked up in the current scope),
     * or declare friend functions in TreeStruct (thus they have access to private members of TreeStruct, including Flat).
     * using Flat = TreeStruct::Flat; is also type access violation, so either make Flat public or use full type name TreeStruct::Flat.
     * cpp committee cooked absolute shit. */
    struct Flat
    {
        Data d;
        Id_t parent_id = 0;
        QList<Id_t> children_id;

        Flat() = default;
        Flat(const TreeStruct& a)
        {
            d = a.d;
            if (!a.parent.expired())
                parent_id = a.parent.lock()->d.id;
            for (const auto& e : a.children)
                children_id.append(e->d.id);
        }
        friend QDataStream& operator<<(QDataStream& stream, const Flat& data);
        friend QDataStream& operator>>(QDataStream& stream, Flat& data);
    };
private:
    QList<Flat> toFlatList() const;
    void fromFlatList(const QList<Flat>& flatList, Id_t this_id = 0);
    QMap<Id_t, Flat> toFlatMap() const;
    void fromFlatMap(const QMap<Id_t, Flat>& flatMap, Id_t this_id = 0);
};

