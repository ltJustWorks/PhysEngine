#include <iostream>
#include <SDL.h>

const char DATA_PATH[] = "C:\\temp\\trajectory.dat";

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

void handleWallCollision(Particle *p, double boxSize, double restitution) {
    // Bounce off walls
    if (p->x < 0) {
        p->x = 0;
        p->vx = -p->vx * restitution;
    } else if (p->x > boxSize) {
        p->x = boxSize;
        p->vx = -p->vx * restitution;
    }
    if (p->y < 0) {
        p->y = 0;
        p->vy = -p->vy * restitution;
    } else if (p->y > boxSize) {
        p->y = boxSize;
        p->vy = -p->vy * restitution;
    }
    if (p->z < 0) {
        p->z = 0;
        p->vz = -p->vz * restitution;
    } else if (p->z > boxSize) {
        p->z = boxSize;
        p->vz = -p->vz * restitution;
    }
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Particle Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 400, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    Particle particle;

    // Initial conditions
    particle.x = 400.0;
    particle.y = 300.0;
    particle.z = 5.0;
    particle.vx = 1.0; // Initial velocity along the x-axis
    particle.vy = 0.5; // Initial velocity along the y-axis
    particle.vz = -0.5; // Initial velocity along the z-axis

    double dt = 0.1; // Time step
    double totalTime = 1200.0; // Total simulation time
    double boxSize = 400.0; // Size of the bounding box
    double restitution = 0.9; // Coefficient of restitution

    FILE *outputFile = fopen(DATA_PATH, "w");
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
        handleWallCollision(&particle, boxSize, restitution);

        // Rendering
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Draw the particle
        //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        //SDL_RenderDrawPoint(renderer, (int)particle.x, (int)particle.y);

        // Draw a filled rectangle (or square) around the particle position
        int rectSize = 5; // Set the size of the rectangle
        SDL_Rect rect = { (int)particle.x - rectSize / 2, (int)particle.y - rectSize / 2, rectSize, rectSize };
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set color to black
        SDL_RenderFillRect(renderer, &rect);

        SDL_RenderPresent(renderer);

        fprintf(outputFile, "%.2f\t%.2f\t%.2f\t%.2f\n", t, particle.x, particle.y, particle.z);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    fclose(outputFile);
    printf("data saved\n");

    return 0;
}

