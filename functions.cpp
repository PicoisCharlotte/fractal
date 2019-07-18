int fin=0, i, x, y;
float Za, temp, Zb, Ca=0, Cb=0;
char C[80]="C=0+0*i";

SDL_Surface *fenetre, *texte;
SDL_Event event;
SDL_Rect positionTexte={10, 10};
SDL_Color couleur={128, 0, 0};
TTF_Font* police;

SDL_Init(SDL_INIT_VIDEO);
TTF_Init();

police=TTF_OpenFont("lucon.ttf", 10);

SDL_WM_SetCaption("Ensemble de Julia Z(n+1)=Z(n)*Z(n)+C", NULL);

fenetre=SDL_SetVideoMode(LARGEUR, HAUTEUR, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
texte=TTF_RenderText_Blended(police, C, couleur);

while (!fin)
{
SDL_WaitEvent(&event);
switch (event.type)
{
case SDL_QUIT:
fin=1;
break;
case SDL_KEYDOWN:
if ( event.key.keysym.sym==SDLK_ESCAPE )
fin=1;
break;
case SDL_MOUSEMOTION:
{
Ca=4*(float)event.motion.x/LARGEUR-2;
Cb=-4*(float)event.motion.y/HAUTEUR+2;

sprintf(C, "C=%.3f+(%.3f)*i", Ca, Cb);
texte=TTF_RenderText_Blended(police, C, couleur);

break;
}
}

SDL_FillRect(fenetre, NULL, SDL_MapRGB(fenetre->format, 0, 0, 0));
SDL_LockSurface(fenetre);

for ( x=0; x<fenetre->w; x++ )
{
for ( y=0; y<fenetre->h; y++ )
{
Za=4*(float)x/LARGEUR-2;
Zb=-4*(float)y/HAUTEUR+2;

/*Zn=Za+i*Zb*/
/*Z(n+1)=Zn²+C
                =(Za+i*Zb)²+Ca+i*Cb
                =Za²+2*Za*i*Zb+(i*Zb)²+Ca+i*Cb
                =Za²-Zb²+Ca + i*(2*Za*Zb+Cb)*/

if ( ( Za==0 && y%2==0 ) || ( Zb==0 && x%2==0 ) )
setPixel(fenetre, x, y, SDL_MapRGB(fenetre->format, 128, 0, 0));
else
{
for ( i=0; Za*Za+Zb*Zb<=4 && i<N; i++ )
{
temp=Za;

Za=Za*Za-Zb*Zb+Ca;
Zb=2*temp*Zb+Cb;
}

setPixel(fenetre, x, y,
        SDL_MapRGB(fenetre->format, 255-i*255/N, 255-i*255/N, 255-i*255/N));
}
}
}

SDL_UnlockSurface(fenetre);
SDL_BlitSurface(texte, NULL, fenetre, &positionTexte);
SDL_Flip(fenetre);
}

SDL_FreeSurface(texte);

TTF_CloseFont(police);

TTF_Quit();
SDL_Quit();

return 0;
}

void setPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp=surface->format->BytesPerPixel;
    Uint8 *p=(Uint8*)surface->pixels+y*surface->pitch+x*bpp;

    switch(bpp)
    {
        case 1:
            *p=pixel;
            break;
        case 2:
            *(Uint16*)p=pixel;
            break;
        case 3:
        {
            if ( SDL_BYTEORDER==SDL_BIG_ENDIAN )
            {
                p[0]=(pixel>>16)&0xff;
                p[1]=(pixel>>8)&0xff;
                p[2]=pixel&0xff;
            }
            else
            {
                p[0]=pixel&0xff;
                p[1]=(pixel>>8)&0xff;
                p[2]=(pixel>>16)&0xff;
            }
            break;
        }
        case 4:
            *(Uint32*)p=pixel;
            break;
    }

    return;