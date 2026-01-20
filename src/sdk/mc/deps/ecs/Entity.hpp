#pragma once
#include <sdk/Predefine.hpp>
#include <entt/entt.hpp>

struct EntityId {
    std::uint32_t rawId;

    [[nodiscard]] constexpr bool operator==(const EntityId& other) const = default;

    [[nodiscard]] constexpr operator std::uint32_t() const {
        return this->rawId;
    }
};

struct EntityIdTraits {
    using value_type = EntityId;

    using entity_type = std::uint32_t;
    using version_type = std::uint16_t;

    static constexpr entity_type entity_mask = 0x3FFFF;
    static constexpr entity_type version_mask = 0x3FFF;
};

template <>
struct entt::entt_traits<EntityId> : public entt::basic_entt_traits<EntityIdTraits> {
    static constexpr std::size_t page_size = 2048;
};

struct IEntityComponent {};

template <std::derived_from<IEntityComponent> Type>
struct entt::component_traits<Type, EntityId> {
    using element_type = Type;
    using entity_type = EntityId;
    static constexpr bool in_place_delete = true;
    static constexpr std::size_t page_size = 128 * !std::is_empty_v<Type>;
};

template <typename Type>
struct entt::storage_type<Type, EntityId> {
    using type = basic_storage<Type, EntityId>;
};

struct EntityRegistry : std::enable_shared_from_this<EntityRegistry> {
    std::string name;
    entt::basic_registry<EntityId> registry;
    uint32_t id;
};

struct EntityContext {
    EntityRegistry& registry;
    entt::basic_registry<EntityId>& enttRegistry;
    EntityId entity;

    template<std::derived_from<IEntityComponent> T>
    [[nodiscard]] T* tryGetComponent() {
        return this->enttRegistry.try_get<T>(this->entity);
    }

    template<std::derived_from<IEntityComponent> T>
    [[nodiscard]] T* tryGetComponent() const {
        return this->enttRegistry.try_get<T>(this->entity);
    }

    template<std::derived_from<IEntityComponent> T>
    [[nodiscard]] bool hasComponent() const {
        return this->enttRegistry.all_of<T>(this->entity);
    }

    template<std::derived_from<IEntityComponent> T>
    T& getOrAddComponent() {
        return this->enttRegistry.get_or_emplace<T>(this->entity);
    }

    template<std::derived_from<IEntityComponent> T>
    void removeComponent() {
        this->enttRegistry.remove<T>(this->entity);
    }
};

template <std::derived_from<IEntityComponent> Type>
struct entt::type_hash<Type> {
    [[nodiscard]] static consteval id_type value() noexcept {
        constexpr auto name = Type::type_name;
        return hashed_string::value(name.data(), name.size());
    }

    [[nodiscard]] consteval operator id_type() const noexcept {
        return value();
    }
};