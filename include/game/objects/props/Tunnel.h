#pragma once
#include "game/objects/base/Object.h"
#include "resources/Resources.h"

class Tunnel : public Object {
public:
    enum Type {
        NORMAL,
        SCALE,
        SLOPE,
    };

    // Convert a string to a Type
    static Type TypeFromString(const std::string& str) {
        static std::unordered_map<std::string, Type> const table = {
            {"NORMAL", NORMAL}, {"SCALE", SCALE}, {"SLOPE", SLOPE}
        };

        auto it = table.find(str);
        if(it != table.end())
            return it->second;

        throw std::runtime_error("Invalid tunnel type: " + str);
    }

    explicit Tunnel(Type type) : type(type) {
        if (type == SCALE) {
            mesh = AcquireMesh("tunnel_scale.obj");
        } else if (type == SLOPE) {
            mesh = AcquireMesh("tunnel_slope.obj");
        } else {
            mesh = AcquireMesh("tunnel.obj");
        }
        shader = AcquireShader("texture");
        texture = AcquireTexture("tunnel.bmp");
    }
    explicit Tunnel(const std::string& typeStr) : Tunnel(TypeFromString(typeStr)) {}

    virtual ~Tunnel() {
    }

    void SetDoor1(Object &portal) const {
        portal.pos = LocalToWorld().MulPoint(Vector3(0, 1, 1));
        portal.euler = euler;
        portal.scale = Vector3(0.6f, 0.999f, 1) * scale.x;
    }

    void SetDoor2(Object &portal) const {
        portal.euler = euler;
        if (type == SCALE) {
            portal.pos = LocalToWorld().MulPoint(Vector3(0, 0.5f, -1));
            portal.scale = Vector3(0.3f, 0.499f, 0.5f) * scale.x;
        } else if (type == SLOPE) {
            portal.pos = LocalToWorld().MulPoint(Vector3(0, -1, -1));
            portal.scale = Vector3(0.6f, 0.999f, 1) * scale.x;
        } else {
            portal.pos = LocalToWorld().MulPoint(Vector3(0, 1, -1));
            portal.scale = Vector3(0.6f, 0.999f, 1) * scale.x;
        }
    }

private:
    Type type;
};
