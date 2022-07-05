#pragma once

#include "program.hh"

std::shared_ptr<Program> long_tunnel(std::string vertex_src,
                                     std::string fragment_src);

std::shared_ptr<Program> amongus(std::string vertex_src,
                                 std::string fragment_src);
