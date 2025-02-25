#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int w, h; // width, height
} Plane;

typedef struct {
    Plane base;
    int d; // depth
} Volume;

void print_plane(Plane *p) {
    printf("W %d, H %d\n", p->w, p->h);
}

void print_volume(Volume *v) {
    printf("W %d, H %d, D %d\n", v->base.w, v->base.h, v->d);
}

Plane *new_plane(int w, int h) {
    Plane *p = malloc(sizeof(Plane));
    p->w = w;
    p->h = h;
    return p;
}
Volume *new_volume(Plane *p, int d) {
    Volume *v = malloc(sizeof(Volume));
    v->base.w = p->w;
    v->base.h = p->h;
    v->d = d;
    return v;
}

Plane *create_plane() {
    Plane* plane = malloc(sizeof(Plane));
    plane->w = 1;
    plane->h = 2;
    return plane;
}

typedef union {
    int i;
    char c[10];
    Plane p;
} Mix;

int main() {
    Plane p = {5, 2};
    Volume v = {p, 3};
    print_plane(&p);
    print_plane((Plane *) &v);
    print_volume(&v);

    Plane *plane = create_plane();
    printf("Width is %d\n", plane->w);

    printf("sizeof(Mix)=%u\n", sizeof(Mix));

    Mix m;
    m.p.h = 3;
    m.p.w = 5;
    printf("m.i=%d\n", m.i);
    print_plane(&m.p);
    strcpy(m.c, "Hello!\n");
    printf("%s", m.c); // %s - format for a string

    return 0;
}