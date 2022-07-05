#include "scene_builder.hh"


/*std::shared_ptr<Program> house(std::string vertex_src,
                                 std::string fragment_src)
{
    Object plane_red("objects/medium_plane.obj", "textures/white.tga",
                     glm::vec3(0, 0, 0), 0.0);
    Object small_wall_1("objects/small_wall.obj", "textures/green.tga",
                        glm::vec3(0, 10, -31), 0.0);
    Object pillar1_red("objects/pillar.obj", "textures/green.tga",
                       glm::vec3(0, 5, 0), 0.0);
    Object small_wall_2("objects/small_wall.obj", "textures/green.tga",
                        glm::vec3(0, 10, -49), 0.0);

    auto scene = std::make_shared<Scene>(glm::vec3(0.0, 10.0, 0.0));

    auto player =
        std::make_shared<Player>(glm::vec3(10, 10, 3.0), glm::vec3(0, 0, -1));
    scene->add_player(player);

}*/


std::shared_ptr<Program> amongus(std::string vertex_src,
                                 std::string fragment_src)
{
    Object plane_red("objects/medium_plane.obj", "textures/white.tga",
                     glm::vec3(0, 0, 0), 0.0);
    Object plane_blue("objects/medium_plane.obj", "textures/white.tga",
                      glm::vec3(1000, 0, 0), 0.0);

    Object pillar1_red("objects/pillar.obj", "textures/green.tga",
                       glm::vec3(0, 5, 0), 0.0);
    Object pillar1_blue("objects/pillar.obj", "textures/green.tga",
                        glm::vec3(1000, 5, 0), 0.0);

    Object pillar2_red("objects/pillar.obj", "textures/green.tga",
                       glm::vec3(0, 5, 10), 0.0);
    Object pillar2_blue("objects/pillar.obj", "textures/green.tga",
                        glm::vec3(1000, 5, 10), 0.0);

    Object amogus_red("objects/amongus.obj", "textures/pierre.tga",
                      glm::vec3(0, 30, 30), 1.0);
    Object amogus_blue("objects/amongus.obj", "textures/pierre_sang.tga",
                       glm::vec3(1000, 30, 30), 1.0);

    std::shared_ptr<Portal> first_portal = std::make_shared<Portal>(
        Portal(5, 10, glm::vec3(1, 5, 5), 90.0, glm::vec3(0, 1, 0)));
    std::shared_ptr<Portal> second_portal = std::make_shared<Portal>(
        Portal(5, 10, glm::vec3(1001, 5, 5), -90.0, glm::vec3(0, 1, 0)));

    std::shared_ptr<Portal> third_portal = std::make_shared<Portal>(
        Portal(5, 10, glm::vec3(-1, 10, 5), -90.0, glm::vec3(0, 1, 0)));
    std::shared_ptr<Portal> fourth_portal = std::make_shared<Portal>(
        Portal(5, 10, glm::vec3(999, 10, 5), 90.0, glm::vec3(0, 1, 0)));
    
    first_portal->set_destination(second_portal);
    second_portal->set_destination(first_portal);

    third_portal->set_destination(fourth_portal);
    fourth_portal->set_destination(third_portal);

    auto scene = std::make_shared<Scene>(glm::vec3(0.0, 10.0, 0.0));

    auto player =
        std::make_shared<Player>(glm::vec3(10, 10, 3.0), glm::vec3(0, 0, -1));
    scene->add_player(player);

    scene->add_portals(first_portal);
    scene->add_portals(second_portal);
    // scene->add_portals(third_portal);
    // scene->add_portals(fourth_portal);

    scene->add_object(std::make_shared<Object>(plane_red));
    scene->add_object(std::make_shared<Object>(plane_blue));
    scene->add_object(std::make_shared<Object>(pillar1_red));
    scene->add_object(std::make_shared<Object>(pillar1_blue));
    scene->add_object(std::make_shared<Object>(pillar2_red));
    scene->add_object(std::make_shared<Object>(pillar2_blue));
    scene->add_object(std::make_shared<Object>(amogus_red));
    scene->add_object(std::make_shared<Object>(amogus_blue));

    std::shared_ptr<Program> prog =
        Program::make_program(vertex_src, fragment_src, scene, player);

    return prog;
}

std::shared_ptr<Program> long_tunnel(std::string vertex_src,
                                     std::string fragment_src)
{
    Object plane("objects/plane.obj", "textures/white.tga", glm::vec3(0, 0, 0),
                 0.0);

    Object long_wall_1("objects/long_wall.obj", "textures/white.tga",
                       glm::vec3(0, 10, 31), 0.0);
    Object long_wall_2("objects/long_wall.obj", "textures/green.tga",
                       glm::vec3(0, 10, 49), 0.0);
    Object long_roof("objects/long_roof.obj", "textures/green.tga",
                     glm::vec3(0, 21, 40), 0.0);

    /*Object plane_far("objects/plane.obj", "textures/white.tga", glm::vec3(0, 750, 0),
                 0.0);

    Object long_wall_1_far("objects/long_wall.obj", "textures/white.tga",
                       glm::vec3(0, 760, 31), 0.0);
    Object long_wall_2_far("objects/long_wall.obj", "textures/green.tga",
                       glm::vec3(0, 760, 49), 0.0);
    Object long_roof_far("objects/long_roof.obj", "textures/green.tga",
                     glm::vec3(0, 21+750, 40), 0.0);
    

    Object small_wall_1("objects/small_wall.obj", "textures/green.tga",
                        glm::vec3(0, 10, -31), 0.0);
    Object small_wall_2("objects/small_wall.obj", "textures/green.tga",
                        glm::vec3(0, 10, -49), 0.0);
    Object small_roof("objects/small_roof.obj", "textures/green.tga",
                      glm::vec3(0, 21, -40), 0.0);
    */

    std::shared_ptr<Portal> first_portal = std::make_shared<Portal>(
        Portal(8, 10, glm::vec3(-95, 10, 40), -90.0, glm::vec3(0, 1, 0)));
    std::shared_ptr<Portal> second_portal = std::make_shared<Portal>(
        Portal(8, 10, glm::vec3(95, 10, 40), 90.0, glm::vec3(0, 1, 0)));

    /*std::shared_ptr<Portal> third_portal = std::make_shared<Portal>(
        Portal(8, 10, glm::vec3(-30, 10, -40), -90.0, glm::vec3(0, 1, 0)));
    std::shared_ptr<Portal> fourth_portal = std::make_shared<Portal>(
        Portal(8, 10, glm::vec3(-90, 760, 40), 90.0, glm::vec3(0, 1, 0)));

    std::shared_ptr<Portal> fifth_portal = std::make_shared<Portal>(
        Portal(8, 10, glm::vec3(30, 10, -40), 90.0, glm::vec3(0, 1, 0)));
    std::shared_ptr<Portal> sixth_portal = std::make_shared<Portal>(
        Portal(8, 10, glm::vec3(90, 760, 40), -90.0, glm::vec3(0, 1, 0)));
    */
    first_portal->set_destination(second_portal);
    second_portal->set_destination(first_portal);

    /*third_portal->set_destination(fourth_portal);
    fourth_portal->set_destination(third_portal);

    fifth_portal->set_destination(sixth_portal);
    sixth_portal->set_destination(fifth_portal);
    */
    auto player =
        std::make_shared<Player>(glm::vec3(0, 0, 3.0), glm::vec3(0, 0, -1));

    auto scene = std::make_shared<Scene>(glm::vec3(0.0, 10.0, 0.0));
    scene->add_portals(first_portal);
    scene->add_portals(second_portal);
    /*scene->add_portals(third_portal);
    scene->add_portals(fourth_portal);
    scene->add_portals(fifth_portal);
    scene->add_portals(sixth_portal);
    */
    scene->add_player(player);

    scene->add_object(std::make_shared<Object>(plane));
    //scene->add_object(std::make_shared<Object>(plane_far));

    scene->add_object(std::make_shared<Object>(long_wall_1));
    scene->add_object(std::make_shared<Object>(long_wall_2));
    scene->add_object(std::make_shared<Object>(long_roof));

    /*scene->add_object(std::make_shared<Object>(long_wall_1_far));
    scene->add_object(std::make_shared<Object>(long_wall_2_far));
    scene->add_object(std::make_shared<Object>(long_roof_far));
    
    scene->add_object(std::make_shared<Object>(small_wall_1));
    scene->add_object(std::make_shared<Object>(small_wall_2));
    scene->add_object(std::make_shared<Object>(small_roof));
    */

    std::shared_ptr<Program> prog =
        Program::make_program(vertex_src, fragment_src, scene, player);

    return prog;
}
