#ifndef PLATFORMER_CUPDATEABLE_H
#define PLATFORMER_CUPDATEABLE_H

class IUpdatable {
public:
    virtual void Tick(float DeltaTime) = 0;
};

#endif //PLATFORMER_CUPDATEABLE_H
