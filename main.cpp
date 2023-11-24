#include <iostream>
#include <stdio.h>

typedef struct {
    double x, y, z; // Position
    double vx, vy, vz; // Velocity
} Particle;

void updatePosition(Particle *p, double dt) {
    // Update position based on velocity
    p->x += p->vx * dt;
    p->y += p->vy * dt;
    p->z += p->vz * dt;
}

void updateVelocity(Particle *p, double ax, double ay, double az, double dt) {
    // Update velocity based on acceleration
    p->vx += ax * dt;
    p->vy += ay * dt;
    p->vz += az * dt;
}

int main() {
    Particle particle;

    // Initial conditions
    particle.x = 0.0;
    particle.y = 0.0;
    particle.z = 0.0;
    particle.vx = 1.0; // Initial velocity along the x-axis
    particle.vy = 0.0;
    particle.vz = 0.0;

    double dt = 0.1; // Time step
    double totalTime = 20.0; // Total simulation time

    FILE *outputFile = fopen("C:\\temp\\trajectory.dat", "w");
    if (outputFile == nullptr) {
        perror("fopen");
        return 1;
    }

    fprintf(outputFile, "Time\tX\tY\tZ\n");
    // Initial positions

    for (double t = 0.0; t <= totalTime; t += dt) {
        // Gravity (assuming Earth-like gravity, modify as needed)
        double ax = 0.0;
        double ay = 0.0;
        double az = -9.8; // Acceleration due to gravity

        updateVelocity(&particle, ax, ay, az, dt);
        updatePosition(&particle, dt);

        fprintf(outputFile, "%.2f\t%.2f\t%.2f\t%.2f\n", t, particle.x, particle.y, particle.z);
    }

    fclose(outputFile);
    printf("data saved\n");

    return 0;
}

