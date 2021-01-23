//
// Created by logan on 21.1.21..
//
#include <iostream>
#include "MoveableObject.h"

MoveableObject::MoveableObject(glm::vec3 pos, glm::vec3 moveDir, float width, float height)
    : shouldDelete(false), currentPosition(pos), movementDir(moveDir)
{
    hitbox = new BoundingBox(pos.x, pos.y, pos.z, width, height);
}

MoveableObject::~MoveableObject() {
    delete hitbox;
}

void MoveableObject::draw(glm::mat4 projection, glm::mat4 view) {
    if(hitbox->shouldShow) {
        hitbox->getVisual()->setup(projection, view);
        hitbox->getVisual()->translate(currentPosition);
        hitbox->getVisual()->scale(glm::vec3(hitbox->width/2, hitbox->height/2, hitbox->width/2));
        hitbox->getVisual()->draw();
    }
}

void MoveableObject::setShow(bool value) {
    hitbox->shouldShow = value;
}

void MoveableObject::move() {
    glm::vec3 newPos = glm::vec3(currentPosition + movementDir);
    hitbox->updateBox(
            newPos.x - currentPosition.x,
            newPos.y - currentPosition.y,
            newPos.z - currentPosition.z
    );
    currentPosition = newPos;
}

