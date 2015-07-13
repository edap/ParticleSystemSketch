#include "particleSystem.h"

ParticleSystem::ParticleSystem(){

}

void ParticleSystem::setup(ofVec3f _pos, int _n_particles){
    pos = _pos;
    n_particles = _n_particles;    
    
    for (int i = 0; i < n_particles; i++) {
        Particle particle(pos);
        particle.setup();
        particle.age = ofRandom(particle.max_age);
        particles.push_back(particle);
        
    }
}

//--------------------------------------------------------------
void ParticleSystem::update(ofVec3f _force,
                            float noiseRandomOffset,
                            float spaceFrequency,
                            float timeFrequency,
                            float noiseMagnitude,
                            float oldVelAmount){
    //time stuff
    float time = ofGetElapsedTimef();
    float noiseReadTime = time * timeFrequency;
    float timeDelta = 1.0 / 60.0;
    for (int i = 0; i < particles.size(); i++) {
        Particle* p = &particles.at(i);
        if (p->age > p->max_age) {
            p->setup();
        }
        p->update(noiseRandomOffset, spaceFrequency,
                  noiseReadTime, timeDelta, timeFrequency,
                  noiseMagnitude, oldVelAmount);
    }
}

void ParticleSystem::updateSnow(){
    float time = ofGetElapsedTimef();
    for (int i = 0; i < particles.size(); i++) {
        Particle* p = &particles.at(i);
//        if (p->age > p->max_age) {
//            p->setup();
//        }
        p->updateSnow(time);
    }
}

//--------------------------------------------------------------
void ParticleSystem::draw(){
    float time = ofGetElapsedTimef() * 0.1;
    ofMesh mesh;
    mesh.setMode( OF_PRIMITIVE_LINES );
    ofMesh* pointer_mesh = &mesh;

    for (int i=0; i<particles.size(); i++) {
        Particle* p = &particles.at(i);
        addParticleToMesh(p, pointer_mesh);
    }
    mesh.draw();
}

void ParticleSystem::addParticleToMesh(Particle *p, ofMesh *mesh){
    if( p->tail.size() > 1 ){
        for( int j = 1; j < p->tail.size(); j++ ){
            //mesh.addColor( tmpCol );
            //mesh->addColor( ofFloatColor(1.0, 0.0, 0.0) );
            mesh->addVertex( p->tail.at(j-1) );
            mesh->addColor( ofFloatColor(1.0, 0.0, 0.0) );
            
            // if you comment out these 2 liness, the each vertex will be connected to the begining of the next one
            // and not to the end of the tail
            mesh->addVertex( p->tail.at(j) );
            mesh->addColor( ofFloatColor(1.0, 0.0, 0.0) );
        }
    }
    
}


void ParticleSystem::resetForces(){
    for (Particle p : particles){
        p.resetForces();
    }
}

