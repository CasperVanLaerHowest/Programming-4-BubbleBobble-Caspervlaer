#pragma once
#include "Scene.h"
#include <glm/ext/vector_float2.hpp>

void CreatePlayer(dae::Scene& scene);

void SpawnBubble(dae::Scene& scene, const glm::vec2& position, bool facingRight);
