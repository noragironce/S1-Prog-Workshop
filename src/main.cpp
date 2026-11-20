#include <sil/sil.hpp>
#include "random.hpp" //pour lexo7 avec l'image bruitée
#include <cmath> //pour lexo 10 avec la luminositée

sil::Image keep_green_only(sil::Image image) // Prend l'image par référence pour pouvoir la modifier
{
    for (glm::vec3& color : image.pixels())
    {
        color.r = 0.f;
        color.b = 0.f;
    }
    return image;
}

sil::Image channels_swap(sil::Image image) // Prend l'image par référence pour pouvoir la modifier
{
    for (glm::vec3& color : image.pixels()){
        std::swap(color.r,color.b);
    }
    return image;
}

sil::Image blackandwhite(sil::Image image)
{
    for(glm::vec3& color : image.pixels()){
       
        float gris = (color.r + color.g + color.b) / 3.f; //on divise par 3 car le gris c'est (0.5,0.5,0.5)
        
        color.r = gris;
        color.g = gris;
        color.b = gris;
    }
    return image;
}

sil::Image negative(sil::Image image)
{
    for(glm::vec3& color : image.pixels()){

        color = 1.f - color;
        
        // color.r = 1.f - color.r;
        // color.g = 1.f - color.g;
        // color.b = 1.f - color.b;
        // ca revient a faire le meme chose, les couleurs sont representes de 0 à 1, pour le blanc cest 1 et pour le noir cest 0 
        // donc color = 1 - blanc = 0 et color = 1 - noir = 1
    }
    return image;
}

sil::Image degrade(){

    sil::Image new_image{300/*width*/, 200/*height*/};

    for (int x{0}; x < new_image.width(); x++)
    {
        for (int y{0}; y < new_image.height(); y++)
        {
            
            //float gris = x / 299.f; // on veut 0 quand x = 0, 0,5 quand x = 150, 1 quand x = 299

            float gris = static_cast<float>(x) / (new_image.width() - 1);
            new_image.pixel(x, y) = glm::vec3{gris, gris, gris};
            
            // image.pixel(x, y).r = gris;
            // image.pixel(x, y).g = gris;
            // image.pixel(x, y).b = gris;
        }
    }
    return new_image;
}

sil::Image miror(sil::Image image){

    for (int x{0}; x < image.width() / 2; x++) // on doit faire diviser par 2 pour pas avoir une boucle infini
    {
        for (int y{0}; y < image.height(); y++)
        {
         std::swap(image.pixel(x, y), image.pixel(image.width() - 1 - x, y));
        }
    }
    return image;
}

sil::Image bruite(sil::Image image){
     
   for(glm::vec3& color : image.pixels()){

        if(true_with_probability(0.2f))
        { //chaque pixel a 20% de chance d'etre true cad d'etre modifier donc 20% de notre image sera modifier et 80% restera intact 
            color.r = random_float(0.f,1.f);//on choisi entre 0 (noir) et 1 (blanc) toute les couleur entre les deux au hasard
            color.g = random_float(0.f,1.f);
            color.b = random_float(0.f,1.f);
        }
    
   }
    // for(glm::vec3& color : image.pixels()){
    //     color.r = random_int(image.width(), image.height());
    //     color.g = random_int(image.width(), image.height());
    //     color.b = random_int(image.width(), image.height());
    // J'ai essayer ca mais ca na pas marcher, jai confondu les couleurs avec la taille de limage, jai generer des nombres trop grand pour que ca soit des couleur, 
    // avec ma boucle actuelle je modifie tour les pixels alors qu'on veut en modifier que certains 
   return image;
}

sil::Image angle(sil::Image image){
   
    int new_width = image.height(); //quand on a tourner on a inverser la largeur et la hauteur 
    int new_height = image.width();
    sil::Image new_image{new_width, new_height};// on met ca avant la boucle parce que sinon on ne peut pas faire new_image.pixel() dans la boucle

    for (int x{0}; x < image.width(); x++) 
    {
        for (int y{0}; y < image.height(); y++)
        {
            int newX = image.height() - 1 - y; //quand on tourne on perd une colonne de x qui corresponder a y 
            int newY = x;
            new_image.pixel(newX, newY) = image.pixel(x, y); //les pixels de la nouvelle image sont les memes que ceux de lancienne image 
        }
    }
    
    
    return new_image;
}

sil::Image split(sil::Image image){

    sil::Image new_image{image.width(), image.height()};

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            if(x + 30 < image.width())
            {
                new_image.pixel(x + 30,y).r = image.pixel(x,y).r;
            }
            if(x - 30 >= 0)
            {
                new_image.pixel(x - 30,y).b = image.pixel(x,y).b; //destination = source !!!
            }
            new_image.pixel(x,y).g = image.pixel(x,y).g;
        }
    }

    return new_image;
}

sil::Image luminositydark(sil::Image image){

    for (glm::vec3& color : image.pixels()){
        color.r = std::pow(color.r, 2.f); //prend la valeur du rouge, multiplie la par elle meme, puis remplace lancienne valeur par le resultat
        color.g = std::pow(color.g, 2.f);//une valeur comprise entre 0 et 1 la mettre au carre la rend plus petite 
        color.b = std::pow(color.b, 2.f);// pour eclaircir une image on utilise une puissance entre 0 et 1  
    }
    return image;
}

sil::Image luminositylight(sil::Image image){

    for (glm::vec3& color : image.pixels()){
        color.r = std::pow(color.r, 0.5f); //prend la valeur du rouge, multiplie la par elle meme, puis remplace lancienne valeur par le resultat
        color.g = std::pow(color.g, 0.5f);//une valeur comprise entre 0 et 1 la mettre au carre la rend plus petite 
        color.b = std::pow(color.b, 0.5f);// pour eclaircir une image on utilise une puissance entre 0 et 1  
    }
    return image;
}

float dist(float x1, float y1, float x2, float y2){ //fonction pour pouvoir faire le disque 
    return std::sqrt(std::pow(x2-x1, 2) + std::pow(y2-y1, 2)); //pytagore
}

sil::Image disque(){
    
    sil::Image new_image{500/*width*/, 500/*height*/};
    
    int centre_x = new_image.width()/2; //on calcule le centre exact
    int centre_y = new_image.height()/2;

    float rayon = 100.f;
    
    for (int x{0}; x < new_image.width(); x++)
    {
        for (int y{0}; y < new_image.height(); y++)
        {
            if(dist(x,y,centre_x,centre_y) < rayon ){//si la distance du centre au point est plus petit que le rayon 
                new_image.pixel(x,y) = glm::vec3 {1.f};
            }


        }
    }
    return new_image;
}

sil::Image cercle(){
    
    sil::Image new_image{500/*width*/, 500/*height*/};
    
    int centre_x = new_image.width()/2; //on calcule le centre exact
    int centre_y = new_image.height()/2;

    float rayon = 100.f;
    float rayon2 = 90.f;
    
    for (int x{0}; x < new_image.width(); x++)
    {
        for (int y{0}; y < new_image.height(); y++)
        {
            if(dist(x,y,centre_x,centre_y) < rayon && rayon2 < dist(x,y,centre_x,centre_y)){//si la distance du centre au point est plus petit que le rayon 
                new_image.pixel(x,y) = glm::vec3 {1.f};
            }


        }
    }
    return new_image;
}

void animation(){ //on ne garde plus sil::Image parce que notre fonction ne retourne pas une image elle sauvegarde plusieur images 
    
    for(int i {0}; i < 20; i++){
    
        sil::Image new_image{500/*width*/, 500/*height*/};

        int centre_x = 30 + i * 30;// 30 est le point de depart et *30 nombre de pixel entre chaque cercle
        int centre_y = new_image.height()/2; //point de depart de laxe des y (la moitie)

        float rayon = 100.f;

        for (int x{0}; x < new_image.width(); x++)
        {
        for (int y{0}; y < new_image.height(); y++)
        {
            if(dist(x,y,centre_x,centre_y) < rayon ){//si la distance du centre au point est plus petit que le rayon 
                new_image.pixel(x,y) = glm::vec3 {1.f};
            }


        }
        }
        new_image.save("output/frame_" + std::to_string(i) + ".png");
    }
}

void cercleCentreDifferent(int centre_x, int centre_y, sil::Image& new_image){ //fonction pour la rosace pour pas devoir reecrire les centres des cercles 
    // on passe image par copie sinon loriginal est toujours modifier et on ne voit rien 

    float rayon = 100.f;
    float rayon2 = 90.f;
    
    for (int x{0}; x < new_image.width(); x++)
    {
        for (int y{0}; y < new_image.height(); y++)
        {
            if(dist(x,y,centre_x,centre_y) < rayon && rayon2 < dist(x,y,centre_x,centre_y)){//si la distance du centre au point est plus petit que le rayon 
                new_image.pixel(x,y) = glm::vec3 {1.f};
            }


        }
    }
}

sil::Image rosace(){

    sil::Image new_image{500/*width*/, 500/*height*/};

    cercleCentreDifferent(new_image.width()/2,new_image.height()/2,new_image);

    float angle60 = 3.14 / 3.f; // on met en radians car std::cos() et std::sin() prennent que en radiant 
    float angle120 = 2*(3.14 / 3.f);
    float angle180 = 3.14;
    float angle240 = -2*(3.14/3.f);
    float angle300 = -3.14/3.f;
    float angle0 = 0;


    cercleCentreDifferent(250 + std::cos(angle60)*100,250 + std::sin(angle60)*100, new_image);// on dit a chaque fois de dessiner le cercle avec ce centre et sur cette image 
    cercleCentreDifferent(250 + std::cos(angle120)*100,250 + std::sin(angle120)*100, new_image);
    cercleCentreDifferent(250 + std::cos(angle180)*100,250 + std::sin(angle180)*100, new_image);
    cercleCentreDifferent(250 + std::cos(angle240)*100,250 + std::sin(angle240)*100, new_image);
    cercleCentreDifferent(250 + std::cos(angle300)*100,250 + std::sin(angle300)*100, new_image);
    cercleCentreDifferent(250 + std::cos(angle0)*100,250 + std::sin(angle0)*100, new_image);
    return new_image;
    
}

sil::Image mosaique(sil::Image image){

    sil::Image new_image{image.width() * 5, image.height() * 5};
    
    for(int i {0}; i < 5; i++){ //selectionne la colonne ou on va copier 
        for(int j {0}; j < 5; j++){ //selectionne la ligne ou on va copier 
            for (int x{0}; x < image.width(); x++) //parcour les pixels de colonne
            {
                for (int y{0}; y < image.height(); y++) //parcour les pixels de ligne
                {
                    new_image.pixel(i * image.width() + x, j * image.height() + y) = image.pixel(x,y);
                    // + x et + y permet de donner la position de depart de l'image 
                }
            }
        }
    }
    return new_image;
}
    

sil::Image glitch(sil::Image image)
{
    for (int n{0}; n < 100; n++) 
    {
        int l = random_int(5, 40); 
        int h = random_int(2, 10); 

        int x_0 = random_int(0, image.width() - l); // coin de a et coin de b
        int y_0 = random_int(0, image.height() - h); // -l et -h pour pas deborder
        int x_1 = random_int(0, image.width() - l);
        int y_1 = random_int(0, image.height() - h);                                                        // x_0 + l ne doit pas dépasser la largeur de l'image, donc x_0 va jusqu'à image.width() - l au maximum
        
        for (int i{x_0}; i < x_0 + l; i++)//+ l pour donner la fin du rectangle 
        {
            for (int j{y_0}; j < y_0 + h; j++)
            {
                std::swap(image.pixel(i, j), image.pixel(i + x_1 - x_0, j + y_1 - y_0));//cest le i donc la position de a plus la distance entre le a (ancien x) et le b (nouveau x)
            }
        }
    }
    return image;
}

float brightness(glm::vec3 color){

    return (color.r * 0.2126 + color.g * 0.7152 + color.b * 0.0722);

}

sil::Image sorting(sil::Image image){
    
    // sil::Image sorted{image.width() , image.height()};

    std::vector<glm::vec3>& v{image.pixels()}; //il faut que je passe par ref sinon je modidifie une copie pas lorigninal 
    std::sort(v.begin(), v.end(), [](glm::vec3 const& color1, glm::vec3 const& color2)
    {
    return brightness(color1) < brightness(color2); // Trie selon la luminosité des couleurs (NB : c'est à vous de coder la fonction `brightness`)
    });

    return image;

}

sil::Image sortinglines(sil::Image image){
    

    std::vector<glm::vec3>& v{image.pixels()}; 

    for (int i{0}; i < image.height(); i++) // on passe sur chaque ligne i = le nombre de ligne
    {
        int start = i * image.width();     
        int end = (i * image.width()) + image.width(); //on part du debut et on ajoute la longueur qui reste 

        std::sort(v.begin() + start, v.begin() + end, [](glm::vec3 const& color1, glm::vec3 const& color2)
        {
            return brightness(color1) < brightness(color2);
        });
    }
    return image;
}

sil::Image sortingrand(sil::Image image){

    std::vector<glm::vec3>& v{image.pixels()}; 

    for(int i{0}; i < 100; i++){
    
        int longueur = random_int(10,image.width());//laa longueur de la ligne quon va modifier 
        int nbligne = random_int(0,image.height() - 1);//les ligne commence a 0 donc il y a 300 lignes mais entre 0 et 299
        int position = random_int(0, image.width() - longueur ); //pour pas depasser on a du enlever la longueur de la trainer vu que la position (x) est nimporte ou 
        
        int start = nbligne * image.width() + position;     
        int end = (nbligne * image.width() + position) + longueur; //on part du debut et on ajoute la longueur quon a random 
        
        std::sort(v.begin() + start, v.begin() + end, [](glm::vec3 const& color1, glm::vec3 const& color2)
        {
            return brightness(color1) < brightness(color2);
        });
    }

    return image;
}

sil::Image degradecolor(){
    
    sil::Image new_image{500/*width*/, 500/*height*/};

    for (int x{0}; x < new_image.width(); x++)
        {
            for (int y{0}; y < new_image.height(); y++)
            {
                float t = static_cast<float>(x) / (new_image.width() - 1); //float gris = x / 299.f; on veut 0 quand x = 0, 0,5 quand x = 150, 1 quand x = 299

                glm::vec3 rouge{1.f, 0.f, 0.f};
                glm::vec3 vert{0.f, 1.f, 0.f};

                new_image.pixel(x, y) = glm::mix(rouge, vert, t);
            }
        }
        return new_image;
}

float srgb_to_linear(float c)
{
    if (c <= 0.04045f)
        return c / 12.92f;
    return std::pow((c + 0.055f) / 1.055f, 2.4f);
}

float linear_to_srgb(float c)
{
    if (c <= 0.0031308f)
        return c * 12.92f;
    return 1.055f * std::pow(c, 1.f / 2.4f) - 0.055f;
}       

glm::vec3 srgb_to_linear(glm::vec3 color)
{
    return glm::vec3{srgb_to_linear(color.r), srgb_to_linear(color.g), srgb_to_linear(color.b)};
}

glm::vec3 linear_to_srgb(glm::vec3 color)
{
    return glm::vec3{linear_to_srgb(color.r), linear_to_srgb(color.g), linear_to_srgb(color.b)};
}

sil::Image degradecolor2(){
    
    sil::Image new_image{500/*width*/, 500/*height*/};
    
    for (int x{0}; x < new_image.width(); x++)
        {
            for (int y{0}; y < new_image.height(); y++)
            {
                float t = static_cast<float>(x) / (new_image.width() - 1); //float gris = x / 299.f; on veut 0 quand x = 0, 0,5 quand x = 150, 1 quand x = 299

                glm::vec3 rouge{1.f, 0.f, 0.f};
                glm::vec3 vert{0.f, 1.f, 0.f};

                new_image.pixel(x, y) = linear_to_srgb(glm::mix(srgb_to_linear(rouge), srgb_to_linear(vert), t));
            }
        }
        return new_image;
}


int main()
{
    //exo1
    
    sil::Image image{"images/logo.png"}; //comme jai passer une image au typde de retour a mes fonctions cree au dessus on na besoin que de ramener image 1 seul fois dans notre main
    keep_green_only(image).save("output/greenonly.png");

    //exo2
    
    channels_swap(image).save("output/swap.png");
    
    //exo3
    
    blackandwhite(image).save("output/blackandwhite.png");
    
    //exo4
    
    negative(image).save("output/negative.png");
    
    //exo5
    
    
    degrade().save("output/degrade.png");
    
    //exo6 
    
    miror(image).save("output/miror.png");
    
    //exo7

    bruite(image).save("output/bruite.png");

    //exo8

    angle(image).save("output/angle.png");

    //exo9

    split(image).save("output/split.png");
    
    //exo 10 

    sil::Image image2{"images/photo.jpg"};
    luminositydark(image2).save("output/luminositydark.png");
    luminositylight(image2).save("output/luminositylight.png");

    //exo11

    disque().save("output/disque.png");

    //exo12

    cercle().save("output/cercle.png");

    //exo13 

    animation();

    //exo14

    rosace().save("output/rosace.png");

    //exo15

    mosaique(image).save("output/mosaique.png");

    //exo16 

    glitch(image).save("output/glitch.png");

    //exo17

    sorting(image).save("output/sorting.png");
    sortinglines(image).save("output/sortinglines.png");
    sortingrand(image).save("output/sortingrand.png");

    //exo18


    degradecolor().save("output/degradecolor.png");
    degradecolor2().save("output/degradecolor2.png");

}