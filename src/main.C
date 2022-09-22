#include<SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <winsock2.h>
#include <windows.h>
#include <iphlpapi.h>
#include <string.h>
#pragma comment(lib, "ws2_32.lib")

#define PI 3.14159265

void SDL_ExitWithError(const char *message);

typedef struct player
{
    double x;
    double y;
    double angle;
    double directionX;
    double directionY;
    double degres;
    int vz;
    int vs;
    int vrr;
    int vrl;
}player;

typedef struct Mur
{
    double x;
    double y;
    double distance;
    double xpythagore;
    double ypythagore;
    double angle3D;
    double x3D;
    int dontshow;
    double distancemult;
    double pointCoteY;
    double pointCoteX;
    double angle;
    double directionX;
    double directionY;
}Mur;

void cr_player(player *j){
    j->x = 480.0;
    j->y = 540.0;
    j->angle = 0.0;
    j->directionX = 0.0;
    j->directionY = 0.0;
    j->degres = PI / 180.0;
    j->vz = 0;
    j->vs = 0;
    j->vrr = 0;
    j->vrl = 0;
}

void cr_mur(Mur *m, double x1, double x2){
    m->x = x1;
    m->y = x2; 
    m->distance = 0.0; 
    m->xpythagore = 0.0;
    m->ypythagore = 0.0;
    m->angle3D = 0.0;
    m->x3D = 0.0;
    m->dontshow = 0;
    m->distancemult = 0.0;
    m->pointCoteY = 0.0;
    m->pointCoteX = 0.0;
    m->angle = 0.0;
    m->directionX = 0.0;
    m->directionY = 0.0;
}

void calcul3D(Mur *m, player *j);
void drawMur(Mur *m, Mur *n, SDL_Renderer *r);
int main(int argc,char *argv[])
{
    //online
    printf("client\n");
    WSADATA WSAData;
    SOCKET sock;
    SOCKADDR_IN syn;
    char buffer[255];
    WSAStartup(MAKEWORD(2,0), &WSAData);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    syn.sin_addr.s_addr = inet_addr("25.43.1.27");
    syn.sin_family = AF_INET;
    syn.sin_port = htons(23);
    connect(sock, (SOCKADDR *)&syn, sizeof(syn));
    u_long iMode = 1;
    ioctlsocket(sock, FIONBIO, &iMode);
    player Ulysse;
    cr_player(&Ulysse);
    Mur Mur1;
    cr_mur(&Mur1, 600.0, 400.0);
    Mur Mur2;
    cr_mur(&Mur2, 600.0, 600.0);
    Mur Mur3;
    cr_mur(&Mur3, 400.0, 400.0);
    Mur Mur4;
    cr_mur(&Mur4, 400.0, 600.0);
    Mur Bot;
    cr_mur(&Bot, 450.0, 450.0);
    Mur Bot2;
    cr_mur(&Bot2, 450.0, 450.0);
    Mur ProjectileUlysse;
    cr_mur(&ProjectileUlysse, 450.0, 450.0);
    Mur ProjectileBot;
    cr_mur(&ProjectileBot, 450.0, 450.0);

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0 )
        SDL_ExitWithError("Initialisation SDL");
    window = SDL_CreateWindow("3D Online Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    /*if(SDL_CreateWindowAndRenderer(1920,1080,0,&window,&renderer) != 0)
        SDL_ExitWithError("Impossible de creer la fenetre et le rendu");*/
    SDL_Surface *imageBot = SDL_LoadBMP("Doom-Marine-Doomguy-a.bmp");
    SDL_Texture* textureBot = SDL_CreateTextureFromSurface(renderer, imageBot);
    SDL_Surface *imageProjectile = SDL_LoadBMP("projectile.bmp");
    SDL_Texture* textureProjectile = SDL_CreateTextureFromSurface(renderer, imageProjectile);
    SDL_ShowCursor(SDL_DISABLE);
    SDL_bool program_launched = SDL_TRUE;
    SDL_Rect dest_rect;
    SDL_Rect projectile_rect;
    SDL_Rect projectileBot_rect;
    int fps = 60;
    int desiredDelta = 1000 / fps;
    double new_x = 0.0;
    double new_y = 0.0;
    char result[256];
    struct sockaddr SenderAddr;
    int SenderAddrSize = sizeof (SenderAddr);
    char *tok;
    int shoot = 0;
    int shootBot = 0;
    double angleProjectile = 0.0;
    double XProjectile = 0.0;
    double YProjectile = 0.0;
    while(program_launched)
    {
        int startloop = SDL_GetTicks();
        SDL_Event event;
        
        printf("Ulysse. x en float: %f\n", Ulysse.x);
        sprintf(result,"%f %f %f %f %f %d",Ulysse.x,Ulysse.y, Ulysse.angle, ProjectileUlysse.x, ProjectileUlysse.y, shoot);
        printf("La variable result : %s\n", result);
        sendto(sock, result, strlen(result)+1, 0, (struct sockaddr*)&syn, sizeof(syn));
        if(recvfrom(sock, buffer, sizeof(buffer),0, &SenderAddr, &SenderAddrSize)> 0){
            tok = strtok(buffer, " ");
            printf("Player 2 X string = %s\n", tok);
            Bot.x = atof(tok);
            tok = strtok(NULL, " ");
            Bot.y = atof(tok);
            tok = strtok(NULL, " ");
            Bot.angle = atof(tok);
            tok = strtok(NULL, " ");
            ProjectileBot.x = atof(tok);
            tok = strtok(NULL, " ");
            ProjectileBot.y = atof(tok);
            tok = strtok(NULL, " ");
            shootBot = atof(tok);
            printf("Player 2 X = %f\n", atof(tok));
        }
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_MOUSEMOTION:
                    int x,y;
                    SDL_GetRelativeMouseState(&x, &y);
                    Ulysse.angle = Ulysse.angle - x;
                    continue;
                case SDL_MOUSEBUTTONDOWN:
                    shoot = 1;
                    
                    continue;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {   
                        case SDLK_z:
                                Ulysse.vz = 1;
                            continue;
                        case SDLK_s:
                                Ulysse.vs =1;
                            continue;
                        case SDLK_d:
                                Ulysse.vrr = 1;
                            continue;
                        case SDLK_q:
                                Ulysse.vrl = 1;
                        default:
                            continue;
                    }
                case SDL_KEYUP:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_z:
                                Ulysse.vz = 0;
                            continue;
                        case SDLK_s:
                                Ulysse.vs =0;
                            continue;
                        case SDLK_d:
                            Ulysse.vrr = 0;
                            continue;
                        case SDLK_q:
                            Ulysse.vrl = 0;
                            
                        default:
                            continue;
                    }
                case SDL_QUIT:
                    program_launched = SDL_FALSE;
                    break;
                default:
                    break;
            }
        }
        
        //SDL_WarpMouseInWindow(window, 50, 50);
        if (Ulysse.vrr == 1){
            Ulysse.x = Ulysse.x - cos((Ulysse.angle+90)*Ulysse.degres) * 3;
            Ulysse.y = Ulysse.y - -sin((Ulysse.angle+90)*Ulysse.degres) * 3;
        }
            
        if (Ulysse.vrl == 1){
            Ulysse.x = Ulysse.x - cos((Ulysse.angle -90)*Ulysse.degres) * 3;
            Ulysse.y = Ulysse.y - -sin((Ulysse.angle -90)*Ulysse.degres) * 3;
        }
            
        if (Ulysse.vz == 1){
            Ulysse.x = Ulysse.x + cos(Ulysse.angle*Ulysse.degres) * 3;
            Ulysse.y = Ulysse.y + -sin(Ulysse.angle*Ulysse.degres) * 3;
        }
        if (Ulysse.vs == 1){
            Ulysse.x = Ulysse.x - cos(Ulysse.angle*Ulysse.degres) * 3;
            Ulysse.y = Ulysse.y - -sin(Ulysse.angle*Ulysse.degres)* 3;
        }
        if (Ulysse.angle > 360)
            Ulysse.angle = 0;
        if (Ulysse.angle < 0)
            Ulysse.angle = 360;
        Ulysse.directionX = cos(Ulysse.angle*Ulysse.degres) * 50 + Ulysse.x;
        Ulysse.directionY = -sin(Ulysse.angle*Ulysse.degres) * 50 + Ulysse.y;
        Bot.directionX = cos(Bot.angle*Ulysse.degres) * 50 + Bot.x;
        Bot.directionY = -sin(Bot.angle*Ulysse.degres) * 50 + Bot.y;
        Bot2.x = Bot.directionX;
        Bot2.y = Bot.directionY;

        if (shoot >= 1){
            if (shoot == 1){
                angleProjectile = Ulysse.angle;
                XProjectile = Ulysse.x;
                YProjectile = Ulysse.y;
            }
            shoot = shoot + 10;
            ProjectileUlysse.x = cos(angleProjectile*Ulysse.degres) * shoot + XProjectile;
            ProjectileUlysse.y = -sin(angleProjectile*Ulysse.degres) * shoot + YProjectile;
            if (shoot > 1000){
                shoot = 0;
            }
            calcul3D(&ProjectileUlysse, &Ulysse);
        }
        if (shootBot >= 1)
        {
            calcul3D(&ProjectileBot, &Ulysse);
        }
        calcul3D(&Mur1, &Ulysse);
        calcul3D(&Mur2, &Ulysse);
        calcul3D(&Mur3, &Ulysse);
        calcul3D(&Mur4, &Ulysse);
        calcul3D(&Bot, &Ulysse);
        calcul3D(&Bot2, &Ulysse);
        if(Bot.distance < 20){
            Bot.distance = 20;
        }
        if(ProjectileUlysse.distance < 20){
            ProjectileUlysse.distance = 20;
        }
        if(ProjectileBot.distance < 20){
            ProjectileBot.distance = 20;
        }
        dest_rect.w = 20000/Bot.distance;
        dest_rect.h = 30000/Bot.distance;
        new_x = Bot.x3D-dest_rect.w/2;
        new_y = 20000/Bot.distance+540 -dest_rect.h;
        dest_rect.x =  new_x;
        dest_rect.y = new_y;
        projectile_rect.w = 10000/ProjectileUlysse.distance;
        projectile_rect.h = 10000/ProjectileUlysse.distance;
        projectile_rect.x = ProjectileUlysse.x3D-projectile_rect.w/2;
        projectile_rect.y = 10000/ProjectileUlysse.distance+540 -projectile_rect.h;

        projectileBot_rect.w = 10000/ProjectileBot.distance;
        projectileBot_rect.h = 10000/ProjectileBot.distance;
        projectileBot_rect.x = ProjectileBot.x3D-projectileBot_rect.w/2;
        projectileBot_rect.y = 10000/ProjectileBot.distance+540 -projectileBot_rect.h;
        
        //printf("%f\n", Bot.distance);
        
        
        
        //printf("%f\n", Mur1.x3D);
        //printf("Ulysse x : %f , Ulysse y : %f, Ulysse point x : %f, Ulysse point y : %f ", Ulysse.x, Ulysse.y, Ulysse.directionX, Ulysse.directionY);
        
        //joueur en 2D 
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255 );
        SDL_RenderDrawLine(renderer, Ulysse.x/3, Ulysse.y/3, Ulysse.directionX/3, Ulysse.directionY/3);
        SDL_RenderDrawLine(renderer, Bot.x/3, Bot.y/3, Bot.directionX/3, Bot.directionY/3);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255 ); 
        SDL_RenderDrawLine(renderer, (Ulysse.x + 10)/3, Ulysse.y/3, (Ulysse.x - 10)/3, Ulysse.y/3);
        SDL_RenderDrawLine(renderer, Ulysse.x/3 ,(Ulysse.y + 10)/3, Ulysse.x/3, (Ulysse.y - 10)/3);
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255 );
        SDL_RenderDrawLine(renderer, (Bot.x + 10)/3, Bot.y/3, (Bot.x - 10)/3, Bot.y/3);
        SDL_RenderDrawLine(renderer, Bot.x/3 ,(Bot.y + 10)/3, Bot.x/3, (Bot.y - 10)/3);

        //2D
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255 );
        SDL_RenderDrawLine(renderer, Mur1.x/3, Mur1.y/3, Mur2.x/3, Mur2.y/3);
        SDL_RenderDrawLine(renderer, Mur3.x/3, Mur3.y/3, Mur4.x/3, Mur4.y/3);
        SDL_RenderDrawLine(renderer, 0 ,1080/3 , 1920/6, 1080/3);
        SDL_RenderDrawLine(renderer, 1920/6 ,1080/3 , 1920/6, 0);

        //3D
        if (ProjectileUlysse.dontshow != 1 && shoot >= 1){
                    SDL_RenderCopy(renderer, textureProjectile, NULL, &projectile_rect);
                    
                }
        if (ProjectileUlysse.dontshow != 1 && shootBot >= 1){
                    SDL_RenderCopy(renderer, textureProjectile, NULL, &projectileBot_rect);
                    
                }
        drawMur(&Mur1, &Mur2, renderer);
        drawMur(&Mur3, &Mur4, renderer);
        if (Bot.dontshow != 1){
            SDL_RenderCopy(renderer, textureBot, NULL, &dest_rect);
            SDL_RenderDrawLine(renderer, Bot.x3D, 5000/Bot.distance+540, Bot2.x3D, 5000/Bot2.distance+540);
        }
        
        //curseur millieu 3D
        SDL_RenderDrawLine(renderer, 950, 540, 970, 540);
        SDL_RenderDrawLine(renderer, 960, 530, 960, 550);
        //rendu de la scene
        SDL_RenderPresent(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255 ); 
        SDL_RenderClear(renderer);

        // delay pour 60 fps
        int delta = SDL_GetTicks() - startloop;
        if(delta < desiredDelta)
        {
            SDL_Delay(desiredDelta - delta);
        }
        

    }

    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS; 
}

void calcul3D(Mur *m, player *j)
{
    m->distance = sqrt(((m->x-j->x)*(m->x-j->x))+((m->y-j->y)*(m->y-j->y)));
    m->xpythagore = m->x-j->x;
    m->ypythagore = m->y-j->y;
    if (m->ypythagore < 0){
        m->angle3D = (acos(m->xpythagore / m->distance) * (180 / PI)) - j->angle ;
        
    }
    else if (m->ypythagore > 0)
        m->angle3D = -(acos(m->xpythagore / m->distance) * (180 / PI)) - j->angle;
    if (m->angle3D > 200){
        m->angle3D = m->angle3D - 360;
    }
    if (m->angle3D < -200){
        m->angle3D = m->angle3D + 360;
    }
    if (m->angle3D > 120 || m->angle3D < -120){
        m->dontshow = 1;
        
    }
    else {
        m->dontshow = 0;
        m->x3D = 960 + -sin(m->angle3D*j->degres) * 1300;
    }
    m->distance = m->distance -(sqrt(m->angle3D*m->angle3D))/2;
    if (m->distance < 0){
        m->distance = 1;
    }
    
}

void drawMur(Mur *m, Mur *n, SDL_Renderer *r){
    if (m->dontshow != 1 && n->dontshow != 1){
            SDL_RenderDrawLine(r, m->x3D, 20000/m->distance+540,  n->x3D, 20000/n->distance+540);
            SDL_RenderDrawLine(r, m->x3D, -20000/m->distance+540,  n->x3D, -20000/n->distance+540);
            
        }
    if (m->dontshow != 1)
        SDL_RenderDrawLine(r, m->x3D, -20000/m->distance+540,  m->x3D, 20000/m->distance+540);
            
    if (n->dontshow != 1) 
        SDL_RenderDrawLine(r, n->x3D, -20000/n->distance+540,  n->x3D, 20000/n->distance+540);
}
//fonction de message d'erreur :
void SDL_ExitWithError(const char *message)
{
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

// mémento https://bit.ly/306MjEV

/*if(SDL_SetRenderDrawColor(renderer, 112, 168, 237, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur pour le rendu");

    if(SDL_RenderDrawPoint(renderer, 100, 450) != 0)    
        SDL_ExitWithError("Impossible de dessiner un point");
    
    if(SDL_RenderDrawLine(renderer, 100, 450, 200, 500) != 0)    
        SDL_ExitWithError("et ce ratio il dessine une ligne ?");

    SDL_Rect rectangle;
    rectangle.x = 300;
    rectangle.y = 300;
    rectangle.w = 200;
    rectangle.h = 120;

    if(SDL_RenderFillRect(renderer, &rectangle) != 0)    
        SDL_ExitWithError("Impossible de dessiner un rectangle"); 

/*SDL_Surface *image = NULL;
    SDL_Texture *texture = NULL;
    image = SDL_LoadBMP("src/download.bmp");
    texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    SDL_Rect rectangle;
    SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h)
    SDL_RenderDrawRect(renderer, Ulysse.boxcolision); 


#include <stdio.h>
#include <math.h>

#define PI 3.14159265

int main () {
   double x, ret, val;

   x = 45.0;
   val = PI / 180;
   ret = sin(x*val);
   printf("The sine of %lf is %lf degrees", x, ret);
   
   return(0);
}

















.\bin\prog.exe                */

// SulfuCorp & GeocraftCorp