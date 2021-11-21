
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


#include <iostream>

int main()
{
    {
        glm::vec3 point(1.0,0.0,0.0);
        glm::vec4 pp(point,1.0);
        // SRT
        glm::mat4 model(1.0);
        model = glm::translate(model,glm::vec3(1,0,0));
        model = glm::rotate(model,glm::radians(45.0f),glm::vec3(0.0,0.0,1.0));
        model = glm::scale(model,glm::vec3(2,0,0));
        pp = model * pp;
        std::cout << glm::to_string(pp) << std::endl;
        // (1.0,0,0)->scale(2,0,0)->(2.0,0,0)->rotate(0,0,1) 45->(1.4,1.4,0)->translate(1.0,0.0,0.0)->(2.4,1.4,0)
    }

    {
        glm::vec3 point(1.0,0.0,0.0);
        glm::vec4 pp(point,1.0);
        // SRT
        glm::mat4 model(1.0);
        model = glm::translate(model,glm::vec3(1,0,0));
        model = glm::rotate(model,glm::radians(45.0f),glm::vec3(0.0,0.0,1.0));
        model = glm::scale(model,glm::vec3(2,0,0));
        pp = model * pp;
        std::cout << glm::to_string(pp) << std::endl;
        // (1.0,0,0)->scale(2,0,0)->(2.0,0,0)->rotate(0,0,1) 45->(1.4,1.4,0)->translate(1.0,0.0,0.0)->(2.4,1.4,0)
    }

    {
        glm::vec3 point(1.0,1.0,1.0);
        glm::vec4 pp(point,1.0f);
        glm::mat4 model(1.0f);
        model = glm::rotate(model,glm::radians(45.0f),glm::vec3(1,1,1));

        pp = model * pp;
        std::cout << glm::to_string(pp) << std::endl;
    }

    {
        glm::vec4 p0(1.0,0.0,0.0,1.0f);
        glm::vec4 p1(2.0,0.0,0.0,1.0f);
        glm::vec4 p2(1.0,1.0,0.0,1.0f);
        glm::vec4 p3(2.0,1.0,0.0,1.0f);

        glm::mat4 model(1.0f);
        model = glm::rotate(model,glm::radians(90.0f),glm::vec3(1,0,1));

        p0 = model * p0;
        p1 = model * p1;
        p2 = model * p2;
        p3 = model * p3;

        std::cout << glm::to_string(p0) << std::endl;
        std::cout << glm::to_string(p1) << std::endl;
        std::cout << glm::to_string(p2) << std::endl;
        std::cout << glm::to_string(p3) << std::endl;
    }

    return 0;
}


