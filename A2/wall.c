#include "wall.h"

void wallSetPosition(struct Wall * wall, int x, int y, int width, int height) {
    wall->x = x;
    wall->y = y;
    wall->width = width;
    wall->height = height;
}

void wallUpdate(SDL_Renderer * renderer, struct Wall * wall){
    SDL_Rect rect = {wall->x, wall->y, wall->width, wall->height};
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderDrawRect(renderer, &rect);
}

//insert link at the first location
void insertFirstWall(struct Wall_collection ** head, int key, struct Wall * wall) {
   //create a link
   // Return a pointer to a structure wall_collection
   struct Wall_collection *link = (struct Wall_collection*) malloc(sizeof(struct Wall_collection));

   // The key is the key
   link->key = key;
   link->wall = *wall; // wall_collection object wall is a pointer to the wall

   //point it to old first node (previous node)
   link->next = *head; // the new node points to the node we passed in

   //point first to new first node
   *head = link; // The new node passes to a type wall collection

   // Linked list is in reverse
}

// head points to the addresses of the walls in wall_collection
void insertAndSetFirstWall(struct Wall_collection ** head, int key, int x, int y, int width, int height){
   // Allocate memory to the size of the wall struct and return a pointer to that location that "wall" points to
   struct Wall *wall = (struct Wall*) malloc(sizeof(struct Wall));
   wallSetPosition(wall, x, y, width, height);
   insertFirstWall(head, key, wall);

}

void updateAllWalls(struct Wall_collection * head, SDL_Renderer * renderer) {
   struct Wall_collection *ptr = head;

   //start from the beginning
   while(ptr != NULL) {
      //printf("(%d)",ptr->key);
      wallUpdate(renderer, &ptr->wall);
      ptr = ptr->next;
   }

}
