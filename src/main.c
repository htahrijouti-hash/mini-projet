#ifdef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>

#define QUIZ_BEST_SCORE_FILE "quiz_best_score.dat"
#define QUESTION_POOL_SIZE 12
#define QUESTIONS_PER_GAME 8
#define TIME_PER_GAME 90
#define POINTS_PER_GOOD_ANSWER 10
#define APP_PHOTO_FILENAME "app_photo.bmp"
#define QUESTION_PHOTO_PATTERN "question_%s_%02d.bmp"
#define QUESTION_PHOTO_FALLBACK_PATTERN "question_%02d.bmp"

#define ID_BTN_1 101
#define ID_BTN_2 102
#define ID_BTN_3 103
#define ID_BTN_4 104
#define ID_BTN_NEW 105
#define ID_TIMER_GAME 201

typedef struct {
    const char *text;
    const char *choices[4];
    int correct_index;
} Question;

typedef enum {
    DOMAIN_VOITURE = 0,
    DOMAIN_SPORT = 1,
    DOMAIN_ECOLE = 2,
    DOMAIN_CODE = 3,
    DOMAIN_HISTOIRE = 4,
    DOMAIN_CINEMA = 5,
    DOMAIN_MUSIQUE = 6,
    DOMAIN_GEOGRAPHIE = 7,
    DOMAIN_JEUXVIDEO = 8,
    DOMAIN_SCIENCE = 9
} Domain;

static const Question QUESTIONS_VOITURE[QUESTION_POOL_SIZE] = {
    {"Quel panneau impose l'arret total ?", {"Stop", "Cedez le passage", "Sens interdit", "Priorite"}, 0},
    {"Que signifie un feu orange fixe ?", {"Passer vite", "Ralentir puis passer", "S'arreter si possible", "Tourner a droite"}, 2},
    {"Vitesse maximale en ville (sauf indication) ?", {"30 km/h", "50 km/h", "70 km/h", "90 km/h"}, 1},
    {"La ceinture est obligatoire...", {"Seulement devant", "Seulement autoroute", "Pour tous les passagers", "Jamais"}, 2},
    {"Avant de changer de voie, il faut...", {"Klaxonner", "Mettre le clignotant", "Freiner sec", "Couper la musique"}, 1},
    {"Un rond-point se prend...", {"Toujours a gauche", "Toujours a droite", "Au hasard", "En marche arriere"}, 1},
    {"Distance de securite: au moins...", {"1 seconde", "2 secondes", "5 secondes", "10 secondes"}, 1},
    {"Sur route mouillee, il faut...", {"Accelerer", "Freiner plus tard", "Augmenter distances", "Rouler plein phares"}, 2},
    {"Que fait l'ABS ?", {"Bloque les freins", "Evite blocage des roues", "Allume les phares", "Coupe moteur"}, 1},
    {"Un passage pieton impose...", {"Priorite pietons", "Priorite voitures", "Stationnement", "Demi-tour"}, 0},
    {"Conduire avec telephone en main est...", {"Autorise", "Interdit", "Autorise en ville", "Obligatoire"}, 1},
    {"Avant un long trajet, verifier...", {"Pneus et freins", "Radio seulement", "Klaxon seulement", "GPS seulement"}, 0}
};

static const Question QUESTIONS_SPORT[QUESTION_POOL_SIZE] = {
    {"Combien de joueurs par equipe en football sur terrain ?", {"9", "10", "11", "12"}, 2},
    {"Dans quel sport fait-on un dunk ?", {"Tennis", "Basket", "Rugby", "Natation"}, 1},
    {"Combien de sets gagnants en tennis masculin Grand Chelem ?", {"2", "3", "4", "5"}, 1},
    {"Un marathon fait environ...", {"21 km", "30 km", "42.195 km", "50 km"}, 2},
    {"Dans le handball, le gardien peut utiliser...", {"Seulement pieds", "Seulement mains", "Tout le corps", "Aucun"}, 2},
    {"Combien de points vaut un panier a 3 points ?", {"1", "2", "3", "4"}, 2},
    {"Le rugby se joue surtout avec un ballon...", {"Rond", "Ovale", "Carre", "Plat"}, 1},
    {"Sport de Kylian Mbappe ?", {"Basket", "Football", "Tennis", "Cyclisme"}, 1},
    {"Dans la natation, le crawl est...", {"Une nage", "Un saut", "Un arbitre", "Un equipement"}, 0},
    {"Le Tour de France est une competition de...", {"Course a pied", "Cyclisme", "Moto", "Voile"}, 1},
    {"Combien de joueurs en volley par equipe sur terrain ?", {"5", "6", "7", "8"}, 1},
    {"Avant le sport, l'echauffement sert a...", {"Fatiguer", "Prevenir blessures", "Dormir", "Manger"}, 1}
};

static const Question QUESTIONS_ECOLE[QUESTION_POOL_SIZE] = {
    {"Combien font 9 x 7 ?", {"56", "63", "72", "49"}, 1},
    {"La capitale du Maroc est...", {"Rabat", "Fes", "Oujda", "Meknes"}, 0},
    {"Le verbe dans 'Nous etudions' est...", {"Nous", "etudions", "dans", "le"}, 1},
    {"H2O correspond a...", {"Oxygene", "Hydrogene", "Eau", "Sel"}, 2},
    {"Combien de continents sur Terre ?", {"5", "6", "7", "8"}, 2},
    {"En informatique, CPU signifie...", {"Unite centrale de traitement", "Carte universelle", "Code principal", "Controleur public"}, 0},
    {"12 + 15 = ?", {"25", "26", "27", "28"}, 2},
    {"Quel est l'antonyme de 'grand' ?", {"haut", "petit", "large", "fort"}, 1},
    {"En histoire, l'ecriture apparait d'abord en...", {"Mesopotamie", "Amerique", "Australie", "Groenland"}, 0},
    {"Le triangle a...", {"2 cotes", "3 cotes", "4 cotes", "5 cotes"}, 1},
    {"Une phrase commence par...", {"une virgule", "une majuscule", "un chiffre", "rien"}, 1},
    {"La photosynthese concerne surtout...", {"les animaux", "les plantes", "les roches", "les oceans"}, 1}
};

static const Question QUESTIONS_CODE[QUESTION_POOL_SIZE] = {
    {"En C, quel fichier contient souvent main ?", {"app.h", "main.c", "index.js", "program.py"}, 1},
    {"Quel symbole commente une ligne en C ?", {"//", "#", "--", "**"}, 0},
    {"Quelle fonction affiche du texte ?", {"scanf", "printf", "malloc", "fopen"}, 1},
    {"Pour comparer l'egalite en C on utilise...", {"=", "==", "===", "=>"}, 1},
    {"Que fait srand(time(NULL)) ?", {"Initialise l'aleatoire", "Ferme le programme", "Trie un tableau", "Sauvegarde"}, 0},
    {"Un tableau en C commence a l'index...", {"-1", "0", "1", "2"}, 1},
    {"Quel mot-cle sort d'une boucle ?", {"continue", "break", "stop", "return"}, 1},
    {"int represente...", {"Un entier", "Un texte", "Un reel", "Un fichier"}, 0},
    {"Quelle extension pour un fichier source C ?", {".cpp", ".py", ".c", ".java"}, 2},
    {"Quel outil compile ici sur Windows ?", {"gcc", "photoshop", "excel", "paint"}, 0},
    {"Combien de bits dans 1 octet ?", {"4", "8", "16", "32"}, 1},
    {"malloc sert a...", {"Allouer la memoire", "Afficher", "Lire clavier", "Quitter"}, 0}
};

static const Question QUESTIONS_HISTOIRE[QUESTION_POOL_SIZE] = {
    {"Les pyramides sont associees a...", {"Egypte ancienne", "Rome", "Grece", "Chine"}, 0},
    {"Qui a peint la Joconde ?", {"Van Gogh", "Da Vinci", "Picasso", "Monet"}, 1},
    {"La Revolution francaise debute en...", {"1789", "1492", "1914", "1960"}, 0},
    {"L'ecriture cuneiforme vient de...", {"Mesopotamie", "Maroc", "Japon", "Bresil"}, 0},
    {"Les pharaons regnaient sur...", {"Rome", "Egypte", "Inde", "Perou"}, 1},
    {"Christophe Colomb traverse l'Atlantique en...", {"1200", "1492", "1700", "2000"}, 1},
    {"La 1ere guerre mondiale commence en...", {"1914", "1939", "1812", "2001"}, 0},
    {"L'Empire romain avait pour capitale...", {"Athenes", "Rome", "Madrid", "Londres"}, 1},
    {"La Grande Muraille se trouve en...", {"Coree", "Chine", "Japon", "Mongolie"}, 1},
    {"Le Moyen Age se situe entre...", {"Antiquite et epoque moderne", "1900 et 2000", "Renaissance et XXIe", "Aucune"}, 0},
    {"Napoleon etait empereur de...", {"France", "Italie", "Allemagne", "Espagne"}, 0},
    {"L'imprimerie moderne est liee a...", {"Gutenberg", "Newton", "Edison", "Tesla"}, 0}
};

static const Question QUESTIONS_CINEMA[QUESTION_POOL_SIZE] = {
    {"Un film est compose de...", {"Sequences et scenes", "Seulement musique", "Pages web", "Fichiers texte"}, 0},
    {"Qui realise un film ?", {"Le realisateur", "Le gardien", "Le cuisinier", "Le vendeur"}, 0},
    {"Le genre 'comedie' vise surtout a...", {"Faire rire", "Faire peur", "Informer meteo", "Documenter code"}, 0},
    {"Dans un cinema, le grand ecran est devant...", {"Le public", "La caisse", "Le parking", "La sortie"}, 0},
    {"La musique d'un film s'appelle...", {"Bande originale", "Bande passante", "Bande papier", "Bande cache"}, 0},
    {"Un acteur principal joue...", {"Un role important", "Toujours figurant", "Seulement pub", "Aucun role"}, 0},
    {"Le mot 'animation' designe souvent...", {"Film dessine/3D", "Journal TV", "Course auto", "Match"}, 0},
    {"Un court-metrage dure en general...", {"Moins long qu'un long-metrage", "10 heures", "Toujours 3 heures", "1 minute fixe"}, 0},
    {"Le generique apparait...", {"Debut ou fin", "Jamais", "Seulement pub", "Sur ticket"}, 0},
    {"Une salle IMAX est connue pour...", {"Grand format image", "Petit ecran", "Silence interdit", "Sans son"}, 0},
    {"Le festival de Cannes est lie a...", {"Cinema", "Football", "Cuisine", "E-sport"}, 0},
    {"Le scenario correspond a...", {"Histoire du film", "Prix du billet", "Plan de la ville", "Contrat internet"}, 0}
};

static const Question QUESTIONS_MUSIQUE[QUESTION_POOL_SIZE] = {
    {"Combien de notes dans la gamme do-re-mi classique ?", {"5", "7", "8", "12"}, 1},
    {"Quel instrument a des touches noires et blanches ?", {"Piano", "Batterie", "Violon", "Flute"}, 0},
    {"Le rythme se mesure souvent en...", {"BPM", "FPS", "KM", "GB"}, 0},
    {"Une chanson sans paroles est dite...", {"Instrumentale", "Muette", "Noire", "Calme"}, 0},
    {"Quel symbole augmente d'un demi-ton ?", {"b", "#", "%", "@"}, 1},
    {"Le tempo rapide est plutot...", {"Lent", "Dynamique", "Silencieux", "Statique"}, 1},
    {"Un groupe de musiciens jouant ensemble forme...", {"Un orchestre", "Un pixel", "Un moteur", "Un serveur"}, 0},
    {"La batterie est un instrument de...", {"Cordes", "Percussion", "Vent", "Clavier"}, 1},
    {"Le micro sert surtout a...", {"Amplifier la voix", "Changer couleur", "Eteindre scene", "Dessiner"}, 0},
    {"Une octave represente...", {"8 notes", "4 notes", "2 notes", "16 notes"}, 0},
    {"Le DJ mixe principalement...", {"Des pistes audio", "Des photos", "Des livres", "Des cartes"}, 0},
    {"Le mot 'solo' signifie...", {"Jouer seul", "Jouer a deux", "Arreter", "Dormir"}, 0}
};

static const Question QUESTIONS_GEOGRAPHIE[QUESTION_POOL_SIZE] = {
    {"Quel est le plus grand ocean ?", {"Atlantique", "Pacifique", "Indien", "Arctique"}, 1},
    {"Le Maroc se situe en...", {"Europe", "Afrique", "Asie", "Amerique"}, 1},
    {"Quel desert est le plus grand desert chaud ?", {"Gobi", "Sahara", "Kalahari", "Atacama"}, 1},
    {"Un pays et une capitale: Maroc - ...", {"Rabat", "Casablanca", "Marrakech", "Fes"}, 0},
    {"Combien y a-t-il de continents ?", {"5", "6", "7", "8"}, 2},
    {"La latitude mesure...", {"Nord-Sud", "Est-Ouest", "Altitude", "Temperature"}, 0},
    {"Un archipel est...", {"Une chaine de montagnes", "Un groupe d'iles", "Un fleuve", "Une foret"}, 1},
    {"Le Nil est...", {"Une montagne", "Un fleuve", "Un desert", "Une ile"}, 1},
    {"Le pole Nord est dans...", {"L'ocean Arctique", "L'Antarctique", "Le Sahara", "L'Amazonie"}, 0},
    {"Le Japon est compose de...", {"Iles", "Uniquement deserts", "Steppes", "Glaciers"}, 0},
    {"La carte politique montre surtout...", {"Les frontieres", "Le climat", "Le relief", "Les etoiles"}, 0},
    {"Un volcan actif peut...", {"Etre totalement froid", "Entrer en eruption", "Devenir une riviere", "Disparaitre en 1 jour"}, 1}
};

static const Question QUESTIONS_JEUXVIDEO[QUESTION_POOL_SIZE] = {
    {"Dans un jeu, 'HP' signifie souvent...", {"Points de vie", "Haute performance", "Heure precise", "Hyper pixel"}, 0},
    {"Un FPS est un jeu de...", {"Course", "Tir a la 1ere personne", "Puzzle", "Sport"}, 1},
    {"Le mot 'boss' designe...", {"Un ennemi principal", "Un menu", "Un bug", "Un score"}, 0},
    {"Une manette sert a...", {"Commander le jeu", "Brancher internet", "Imprimer", "Stocker photos"}, 0},
    {"Le multiplayer permet...", {"Jouer a plusieurs", "Jouer hors ligne seulement", "Aucun son", "Aucun score"}, 0},
    {"Le 'lag' est un probleme de...", {"Connexion/performance", "Couleur", "Musique", "Batterie TV"}, 0},
    {"Un RPG met souvent l'accent sur...", {"Scenario et evolution", "Seulement vitesse", "Aucun personnage", "Tableurs"}, 0},
    {"Un jeu 'open world' propose...", {"Monde plus libre", "Niveau unique", "Sans carte", "Sans mission"}, 0},
    {"Le checkpoint sert a...", {"Sauvegarder progression", "Supprimer compte", "Changer console", "Couper son"}, 0},
    {"Un 'skin' change surtout...", {"L'apparence", "La connexion", "Le prix", "Le clavier"}, 0},
    {"La difficulte 'hard' est...", {"Plus difficile", "Plus facile", "Sans ennemis", "Sans score"}, 0},
    {"Un jeu inde est souvent...", {"Developpe par petite equipe", "Obligatoirement AAA", "Toujours gratuit", "Toujours mobile"}, 0}
};

static const Question QUESTIONS_SCIENCE[QUESTION_POOL_SIZE] = {
    {"L'eau bout a environ...", {"50 C", "75 C", "100 C", "120 C"}, 2},
    {"La Terre tourne autour...", {"De la Lune", "Du Soleil", "De Mars", "De Venus"}, 1},
    {"Le symbole chimique de l'oxygene est...", {"O", "Ox", "Og", "Oy"}, 0},
    {"Le corps humain a besoin d'oxygene pour...", {"Respirer", "Dormir seulement", "Ecrire", "Nager vite"}, 0},
    {"Une cellule est...", {"Une unite du vivant", "Une planete", "Un metal", "Un volcan"}, 0},
    {"La force qui nous attire vers le sol est...", {"Magnetisme", "Gravite", "Pression", "Vitesse"}, 1},
    {"L'unite de temperature la plus courante est...", {"Metre", "Seconde", "Celsius", "Watt"}, 2},
    {"Un atome contient notamment...", {"Noyau et electrons", "Seulement eau", "Pixels", "Routes"}, 0},
    {"Le systeme solaire appartient a...", {"La Voie lactee", "Andromede uniquement", "Aucune galaxie", "Le Sahara"}, 0},
    {"La photosynthese utilise surtout...", {"Lumiere", "Sable", "Metal", "Carburant"}, 0},
    {"Un circuit electrique a besoin d'un...", {"Chemin ferme", "Chemin ouvert", "Tube vide", "Ecran"}, 0},
    {"La vitesse de la lumiere est...", {"Tres elevee", "Tres lente", "Nulle", "Variable au hasard"}, 0}
};

static const Question *get_domain_pool(Domain domain) {
    if (domain == DOMAIN_VOITURE) {
        return QUESTIONS_VOITURE;
    }
    if (domain == DOMAIN_SPORT) {
        return QUESTIONS_SPORT;
    }
    if (domain == DOMAIN_CODE) {
        return QUESTIONS_CODE;
    }
    if (domain == DOMAIN_HISTOIRE) {
        return QUESTIONS_HISTOIRE;
    }
    if (domain == DOMAIN_CINEMA) {
        return QUESTIONS_CINEMA;
    }
    if (domain == DOMAIN_MUSIQUE) {
        return QUESTIONS_MUSIQUE;
    }
    if (domain == DOMAIN_GEOGRAPHIE) {
        return QUESTIONS_GEOGRAPHIE;
    }
    if (domain == DOMAIN_JEUXVIDEO) {
        return QUESTIONS_JEUXVIDEO;
    }
    if (domain == DOMAIN_SCIENCE) {
        return QUESTIONS_SCIENCE;
    }
    return QUESTIONS_ECOLE;
}

static const char *get_domain_label(Domain domain) {
    if (domain == DOMAIN_VOITURE) {
        return "Voiture";
    }
    if (domain == DOMAIN_SPORT) {
        return "Sport";
    }
    if (domain == DOMAIN_CODE) {
        return "Code";
    }
    if (domain == DOMAIN_HISTOIRE) {
        return "Histoire";
    }
    if (domain == DOMAIN_CINEMA) {
        return "Cinema";
    }
    if (domain == DOMAIN_MUSIQUE) {
        return "Musique";
    }
    if (domain == DOMAIN_GEOGRAPHIE) {
        return "Geographie";
    }
    if (domain == DOMAIN_JEUXVIDEO) {
        return "Jeux Video";
    }
    if (domain == DOMAIN_SCIENCE) {
        return "Science";
    }
    return "Ecole";
}

static const char *get_domain_key(Domain domain) {
    if (domain == DOMAIN_VOITURE) {
        return "voiture";
    }
    if (domain == DOMAIN_SPORT) {
        return "sport";
    }
    if (domain == DOMAIN_CODE) {
        return "code";
    }
    if (domain == DOMAIN_HISTOIRE) {
        return "histoire";
    }
    if (domain == DOMAIN_CINEMA) {
        return "cinema";
    }
    if (domain == DOMAIN_MUSIQUE) {
        return "musique";
    }
    if (domain == DOMAIN_GEOGRAPHIE) {
        return "geographie";
    }
    if (domain == DOMAIN_JEUXVIDEO) {
        return "jeuxvideo";
    }
    if (domain == DOMAIN_SCIENCE) {
        return "science";
    }
    return "ecole";
}

static Domain ask_domain(HWND hwnd) {
    if (MessageBoxA(hwnd, "Tu veux le domaine Voiture ?", "Choix du domaine 1/10", MB_ICONQUESTION | MB_YESNO) == IDYES) {
        return DOMAIN_VOITURE;
    }
    if (MessageBoxA(hwnd, "Tu veux le domaine Sport ?", "Choix du domaine 2/10", MB_ICONQUESTION | MB_YESNO) == IDYES) {
        return DOMAIN_SPORT;
    }
    if (MessageBoxA(hwnd, "Tu veux le domaine Ecole ?", "Choix du domaine 3/10", MB_ICONQUESTION | MB_YESNO) == IDYES) {
        return DOMAIN_ECOLE;
    }
    if (MessageBoxA(hwnd, "Tu veux le domaine Code ?", "Choix du domaine 4/10", MB_ICONQUESTION | MB_YESNO) == IDYES) {
        return DOMAIN_CODE;
    }
    if (MessageBoxA(hwnd, "Tu veux le domaine Histoire ?", "Choix du domaine 5/10", MB_ICONQUESTION | MB_YESNO) == IDYES) {
        return DOMAIN_HISTOIRE;
    }
    if (MessageBoxA(hwnd, "Tu veux le domaine Cinema ?", "Choix du domaine 6/10", MB_ICONQUESTION | MB_YESNO) == IDYES) {
        return DOMAIN_CINEMA;
    }
    if (MessageBoxA(hwnd, "Tu veux le domaine Musique ?", "Choix du domaine 7/10", MB_ICONQUESTION | MB_YESNO) == IDYES) {
        return DOMAIN_MUSIQUE;
    }
    if (MessageBoxA(hwnd, "Tu veux le domaine Geographie ?", "Choix du domaine 8/10", MB_ICONQUESTION | MB_YESNO) == IDYES) {
        return DOMAIN_GEOGRAPHIE;
    }
    if (MessageBoxA(hwnd, "Tu veux le domaine Jeux Video ?", "Choix du domaine 9/10", MB_ICONQUESTION | MB_YESNO) == IDYES) {
        return DOMAIN_JEUXVIDEO;
    }
    if (MessageBoxA(hwnd, "Tu veux le domaine Science ?", "Choix du domaine 10/10", MB_ICONQUESTION | MB_YESNO) == IDYES) {
        return DOMAIN_SCIENCE;
    }

    MessageBoxA(hwnd, "Aucun domaine choisi. Domaine par defaut: Ecole.", "Information", MB_ICONINFORMATION | MB_OK);
    return DOMAIN_ECOLE;
}

static HWND g_lbl_domain;
static const Question *g_active_questions;
static Domain g_current_domain;

static HWND g_lbl_title;
static HWND g_lbl_status;
static HWND g_lbl_question;
static HWND g_img_app_photo;
static HWND g_img_question_photo;
static HWND g_lbl_app_photo_hint;
static HWND g_lbl_question_photo_hint;
static HWND g_btn_answers[4];
static HWND g_btn_new_game;
static HFONT g_font_title;
static HFONT g_font_text;
static HBRUSH g_bg_brush;
static HBITMAP g_app_photo_bitmap;
static HBITMAP g_question_photo_bitmap;

static int g_best_score;
static int g_score;
static int g_current_question;
static int g_time_left;
static int g_order[QUESTION_POOL_SIZE];
static int g_game_running;

static int build_path_from_exe_folder(char *out_path, size_t out_size, const char *filename) {
    char module_path[MAX_PATH];
    char *last_sep;

    if (GetModuleFileNameA(NULL, module_path, MAX_PATH) == 0) {
        return 0;
    }

    last_sep = strrchr(module_path, '\\');
    if (last_sep == NULL) {
        return 0;
    }

    *(last_sep + 1) = '\0';
    if (strlen(module_path) + strlen(filename) + 1 > out_size) {
        return 0;
    }

    strcpy(out_path, module_path);
    strcat(out_path, filename);
    return 1;
}

static void set_bitmap_on_control(HWND control, HBITMAP *target_bitmap, const char *file_path, int width, int height) {
    HBITMAP bitmap = (HBITMAP)LoadImageA(NULL, file_path, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);

    if (*target_bitmap != NULL) {
        DeleteObject(*target_bitmap);
        *target_bitmap = NULL;
    }

    if (bitmap != NULL) {
        *target_bitmap = bitmap;
        SendMessage(control, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bitmap);
    } else {
        SendMessage(control, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
    }
}

static void load_app_photo(void) {
    char path[MAX_PATH];

    if (!build_path_from_exe_folder(path, sizeof(path), APP_PHOTO_FILENAME)) {
        SetWindowTextA(g_lbl_app_photo_hint, "Photo app: chemin introuvable");
        return;
    }

    set_bitmap_on_control(g_img_app_photo, &g_app_photo_bitmap, path, 220, 120);
    if (g_app_photo_bitmap != NULL) {
        SetWindowTextA(g_lbl_app_photo_hint, "Photo app: app_photo.bmp");
    } else {
        SetWindowTextA(g_lbl_app_photo_hint, "Ajoute app_photo.bmp a cote du .exe");
    }
}

static void load_question_photo(int question_one_based_index) {
    char filename[64];
    char path[MAX_PATH];
    char fallback_filename[64];

    snprintf(filename, sizeof(filename), QUESTION_PHOTO_PATTERN, get_domain_key(g_current_domain), question_one_based_index);
    if (!build_path_from_exe_folder(path, sizeof(path), filename)) {
        SetWindowTextA(g_lbl_question_photo_hint, "Photo question: chemin introuvable");
        return;
    }

    set_bitmap_on_control(g_img_question_photo, &g_question_photo_bitmap, path, 300, 220);
    if (g_question_photo_bitmap == NULL) {
        snprintf(fallback_filename, sizeof(fallback_filename), QUESTION_PHOTO_FALLBACK_PATTERN, question_one_based_index);
        if (build_path_from_exe_folder(path, sizeof(path), fallback_filename)) {
            set_bitmap_on_control(g_img_question_photo, &g_question_photo_bitmap, path, 300, 220);
        }
    }

    if (g_question_photo_bitmap != NULL) {
        char msg[96];
        snprintf(msg, sizeof(msg), "Photo question: %s", filename);
        SetWindowTextA(g_lbl_question_photo_hint, msg);
    } else {
        char msg[128];
        snprintf(msg, sizeof(msg), "Image manquante: %s", filename);
        SetWindowTextA(g_lbl_question_photo_hint, msg);
    }
}

static int load_best_score(void) {
    FILE *file = fopen(QUIZ_BEST_SCORE_FILE, "r");
    int best = 0;
    if (file == NULL) {
        return 0;
    }
    if (fscanf(file, "%d", &best) != 1) {
        best = 0;
    }
    fclose(file);
    return best;
}

static void save_best_score(int best) {
    FILE *file = fopen(QUIZ_BEST_SCORE_FILE, "w");
    if (file == NULL) {
        return;
    }
    fprintf(file, "%d\n", best);
    fclose(file);
}

static void shuffle_order(void) {
    int i;
    for (i = 0; i < QUESTION_POOL_SIZE; i++) {
        g_order[i] = i;
    }
    for (i = QUESTION_POOL_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = g_order[i];
        g_order[i] = g_order[j];
        g_order[j] = tmp;
    }
}

static void set_answers_enabled(int enabled) {
    int i;
    for (i = 0; i < 4; i++) {
        EnableWindow(g_btn_answers[i], enabled);
    }
}

static void update_status(void) {
    char buffer[256];
    snprintf(
        buffer,
        sizeof(buffer),
        "Domaine: %s  |  Score: %d  |  Meilleur: %d  |  Temps: %d sec  |  Question: %d/%d",
        get_domain_label(g_current_domain),
        g_score,
        g_best_score,
        g_time_left,
        g_current_question + 1,
        QUESTIONS_PER_GAME
    );
    SetWindowTextA(g_lbl_status, buffer);
}

static void show_question(void) {
    const Question *q;
    int idx;
    int i;

    if (g_current_question >= QUESTIONS_PER_GAME) {
        return;
    }

    idx = g_order[g_current_question];
    q = &g_active_questions[idx];

    SetWindowTextA(g_lbl_question, q->text);
    for (i = 0; i < 4; i++) {
        SetWindowTextA(g_btn_answers[i], q->choices[i]);
    }

    load_question_photo(idx + 1);
    update_status();
}

static void finish_game(HWND hwnd, int timed_out) {
    char result[512];
    int msg_box_result;

    g_game_running = 0;
    KillTimer(hwnd, ID_TIMER_GAME);
    set_answers_enabled(0);

    if (g_score > g_best_score) {
        g_best_score = g_score;
        save_best_score(g_best_score);
    }

    if (timed_out) {
        snprintf(result, sizeof(result),
                 "Temps termine !\n\nScore final: %d\nMeilleur score: %d\n\nRejouer ?",
                 g_score, g_best_score);
    } else {
        snprintf(result, sizeof(result),
                 "Partie finie !\n\nScore final: %d\nMeilleur score: %d\n\nRejouer ?",
                 g_score, g_best_score);
    }

    update_status();
    SetWindowTextA(g_lbl_question, "Partie terminee. Clique sur 'Nouvelle Partie' ou choisis Rejouer.");
    SetWindowTextA(g_btn_answers[0], "-");
    SetWindowTextA(g_btn_answers[1], "-");
    SetWindowTextA(g_btn_answers[2], "-");
    SetWindowTextA(g_btn_answers[3], "-");
    SetWindowTextA(g_lbl_question_photo_hint, "Photos actives a la prochaine partie");

    msg_box_result = MessageBoxA(hwnd, result, "Le Meilleur Quiz de H.M", MB_ICONINFORMATION | MB_YESNO);
    if (msg_box_result == IDYES) {
        SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(ID_BTN_NEW, BN_CLICKED), 0);
    }
}

static void start_new_game(HWND hwnd) {
    char domain_text[128];

    snprintf(domain_text, sizeof(domain_text), "Domaine choisi: %s", get_domain_label(g_current_domain));
    SetWindowTextA(g_lbl_domain, domain_text);

    g_score = 0;
    g_current_question = 0;
    g_time_left = TIME_PER_GAME;
    g_game_running = 1;

    shuffle_order();
    set_answers_enabled(1);
    show_question();

    KillTimer(hwnd, ID_TIMER_GAME);
    SetTimer(hwnd, ID_TIMER_GAME, 1000, NULL);
}

static void submit_answer(HWND hwnd, int answer_index) {
    const Question *q;
    int idx;

    if (!g_game_running) {
        return;
    }

    idx = g_order[g_current_question];
    q = &g_active_questions[idx];

    if (answer_index == q->correct_index) {
        g_score += POINTS_PER_GOOD_ANSWER;
    }

    g_current_question++;

    if (g_current_question >= QUESTIONS_PER_GAME) {
        finish_game(hwnd, 0);
        return;
    }

    show_question();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        g_bg_brush = CreateSolidBrush(RGB(8, 14, 8));
        g_font_title = CreateFontA(28, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET,
                                   OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                   FF_MODERN, "Consolas");
        g_font_text = CreateFontA(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
                                  OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                  FF_MODERN, "Consolas");

        g_lbl_title = CreateWindowA("STATIC", "LE MEILLEUR QUIZ DE H.M // SNAKE STYLE",
                                    WS_VISIBLE | WS_CHILD,
                                                                        24, 18, 860, 38, hwnd, NULL, NULL, NULL);
        g_lbl_status = CreateWindowA("STATIC", "",
                                     WS_VISIBLE | WS_CHILD,
                                                                         24, 66, 860, 30, hwnd, NULL, NULL, NULL);
                g_lbl_domain = CreateWindowA("STATIC", "Domaine choisi: -",
                                                                         WS_VISIBLE | WS_CHILD,
                                                                         24, 92, 510, 24, hwnd, NULL, NULL, NULL);
        g_lbl_question = CreateWindowA("STATIC", "",
                                       WS_VISIBLE | WS_CHILD,
                                                                             24, 120, 510, 70, hwnd, NULL, NULL, NULL);

                g_img_app_photo = CreateWindowA("STATIC", "",
                                                                                WS_VISIBLE | WS_CHILD | SS_BITMAP | WS_BORDER,
                                                                                560, 112, 300, 120, hwnd, NULL, NULL, NULL);
                g_lbl_app_photo_hint = CreateWindowA("STATIC", "",
                                                                                         WS_VISIBLE | WS_CHILD,
                                                                                         560, 238, 300, 24, hwnd, NULL, NULL, NULL);

                g_img_question_photo = CreateWindowA("STATIC", "",
                                                                                         WS_VISIBLE | WS_CHILD | SS_BITMAP | WS_BORDER,
                                                                                         560, 268, 300, 220, hwnd, NULL, NULL, NULL);
                g_lbl_question_photo_hint = CreateWindowA("STATIC", "",
                                                                                                    WS_VISIBLE | WS_CHILD,
                                                                                                    560, 494, 300, 24, hwnd, NULL, NULL, NULL);

        g_btn_answers[0] = CreateWindowA("BUTTON", "",
                                         WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                         24, 210, 240, 52, hwnd, (HMENU)ID_BTN_1, NULL, NULL);
        g_btn_answers[1] = CreateWindowA("BUTTON", "",
                                         WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                         284, 210, 240, 52, hwnd, (HMENU)ID_BTN_2, NULL, NULL);
        g_btn_answers[2] = CreateWindowA("BUTTON", "",
                                         WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                         24, 282, 240, 52, hwnd, (HMENU)ID_BTN_3, NULL, NULL);
        g_btn_answers[3] = CreateWindowA("BUTTON", "",
                                         WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                         284, 282, 240, 52, hwnd, (HMENU)ID_BTN_4, NULL, NULL);

        g_btn_new_game = CreateWindowA("BUTTON", "Nouvelle Partie",
                                       WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                       24, 366, 240, 44, hwnd, (HMENU)ID_BTN_NEW, NULL, NULL);

        SendMessage(g_lbl_title, WM_SETFONT, (WPARAM)g_font_title, TRUE);
        SendMessage(g_lbl_status, WM_SETFONT, (WPARAM)g_font_text, TRUE);
        SendMessage(g_lbl_domain, WM_SETFONT, (WPARAM)g_font_text, TRUE);
        SendMessage(g_lbl_question, WM_SETFONT, (WPARAM)g_font_text, TRUE);
        SendMessage(g_lbl_app_photo_hint, WM_SETFONT, (WPARAM)g_font_text, TRUE);
        SendMessage(g_lbl_question_photo_hint, WM_SETFONT, (WPARAM)g_font_text, TRUE);
        SendMessage(g_btn_answers[0], WM_SETFONT, (WPARAM)g_font_text, TRUE);
        SendMessage(g_btn_answers[1], WM_SETFONT, (WPARAM)g_font_text, TRUE);
        SendMessage(g_btn_answers[2], WM_SETFONT, (WPARAM)g_font_text, TRUE);
        SendMessage(g_btn_answers[3], WM_SETFONT, (WPARAM)g_font_text, TRUE);
        SendMessage(g_btn_new_game, WM_SETFONT, (WPARAM)g_font_text, TRUE);

        load_app_photo();
        g_active_questions = get_domain_pool(DOMAIN_VOITURE);
        g_current_domain = DOMAIN_VOITURE;

        start_new_game(hwnd);
        return 0;
    }

    case WM_TIMER:
        if (wParam == ID_TIMER_GAME && g_game_running) {
            g_time_left--;
            if (g_time_left <= 0) {
                g_time_left = 0;
                update_status();
                finish_game(hwnd, 1);
            } else {
                update_status();
            }
        }
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_BTN_1:
            submit_answer(hwnd, 0);
            return 0;
        case ID_BTN_2:
            submit_answer(hwnd, 1);
            return 0;
        case ID_BTN_3:
            submit_answer(hwnd, 2);
            return 0;
        case ID_BTN_4:
            submit_answer(hwnd, 3);
            return 0;
        case ID_BTN_NEW:
            start_new_game(hwnd);
            return 0;
        default:
            break;
        }
        break;

    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLORBTN: {
        HDC hdc = (HDC)wParam;
        SetTextColor(hdc, RGB(90, 255, 120));
        SetBkColor(hdc, RGB(8, 14, 8));
        return (LRESULT)g_bg_brush;
    }

    case WM_DESTROY:
        KillTimer(hwnd, ID_TIMER_GAME);
        if (g_font_title != NULL) {
            DeleteObject(g_font_title);
        }
        if (g_font_text != NULL) {
            DeleteObject(g_font_text);
        }
        if (g_bg_brush != NULL) {
            DeleteObject(g_bg_brush);
        }
        if (g_app_photo_bitmap != NULL) {
            DeleteObject(g_app_photo_bitmap);
        }
        if (g_question_photo_bitmap != NULL) {
            DeleteObject(g_question_photo_bitmap);
        }
        PostQuitMessage(0);
        return 0;
    default:
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nShowCmd) {
    WNDCLASSEXA wc;
    HWND hwnd;
    MSG msg;

    (void)hPrev;
    (void)lpCmdLine;

    srand((unsigned int)time(NULL));
    g_best_score = load_best_score();

    g_current_domain = ask_domain(NULL);
    g_active_questions = get_domain_pool(g_current_domain);

    if (MessageBoxA(NULL,
                    "Tu es pret pour Le Meilleur Quiz de H.M ?\n\nCliquer OUI pour commencer.",
                    "Le Meilleur Quiz de H.M",
                    MB_ICONQUESTION | MB_YESNO) != IDYES) {
        return 0;
    }

    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "LeMeilleurQuizWindow";

    if (!RegisterClassExA(&wc)) {
        MessageBoxA(NULL, "Erreur de creation de classe de fenetre.", "Erreur", MB_ICONERROR);
        return 1;
    }

    hwnd = CreateWindowExA(0,
                           "LeMeilleurQuizWindow",
                           "Le Meilleur Quiz de H.M",
                           WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                           CW_USEDEFAULT, CW_USEDEFAULT, 900, 570,
                           NULL, NULL, hInst, NULL);
    if (hwnd == NULL) {
        MessageBoxA(NULL, "Erreur de creation de fenetre.", "Erreur", MB_ICONERROR);
        return 1;
    }

    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

#else
#include <stdio.h>

int main(void) {
    printf("Cette version de l'app est disponible sur Windows (Win32 GUI).\n");
    return 0;
}
#endif
