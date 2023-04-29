#include "PGN.hpp"
#include <iostream>
#include <getopt.h>
// #include <fstream>
// #include <sstream>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
#include <unistd.h>
using namespace pgnp;

#define BLACK "\033[0;30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define CYAN "\033[0;36m"
#define RESET "\033[0m"

void NAGindex (std::string NAG[256]);

int main (int argc, char *argv[]) {

    #ifdef _WIN32
        printf(ROUGE"This program is not compatible with Windows systems.\n");
        return EXIT_FAILURE;
    #endif

    std::string file_input;
    std::string file_output;
    bool output = false;

    // Nombre désiré de count par défaut avant affichage du plateau visuel
    int hmperMain = 4;

    bool debug = false, showNAG = true;



    /// --- ARGUMENTS DE COMMANDES ---
    struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {"input", required_argument, NULL, 'i'},
        {"output", required_argument, NULL, 'o'},
        {"counts", required_argument, NULL, 'c'},
        {"no-nags", no_argument, NULL, 'n'},
        {"stats", no_argument, NULL, 's'},
        {"debug", no_argument, NULL, 'd'},
        {"version", no_argument, NULL, 'v'},
        {0, 0, 0, 0}
    };

    int opt, option_index = 0;

    while ((opt = getopt_long(argc, argv, "i:o:n:sdhv", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'i' :                                                                                            // entrée
                file_input = optarg;

                std::cout << YELLOW "Input file name : " RESET << file_input << std::endl;
                break;

            case 'o' :                                                                                            // sortie
                output = true;
                file_output = optarg;
                std::cout << YELLOW "Output file name : " RESET << file_output << std::endl;
                break;

            case 'c' :                                                                                            // nombre de cout par mainline
                hmperMain = atoi(optarg);
                break;

            case 'n' :                                                                                            // ne pas afficher les NAGs
                showNAG = false;
                break;

            case 's' :                                                                                            // statistiques du fichier pgn
                puts("in -o");
                return(EXIT_SUCCESS);
                break;

            case 'd' :                                                                                            // mode debug
                std::cout << "DEBUG" << std::endl;
                debug = true;
                return(EXIT_SUCCESS);
                break;

            case 'h' :                                                                                            // afficher l'aide
                std::cout << "\nAuthors : " YELLOW "Software made by Antoine DORO, Lukas BOYER and Cylian HOUTMANN.\n" RESET << std::endl;
                std::cout << "Description : " YELLOW "BlackTeX is a trivial PGN to TeX converter. You simply need to input a PGN file, and it will print you a beautiful TeX document of your chess games. You can also indicate an output and the number of counts before showing a chessboard.\n" RESET << std::endl;
                
                std::cout << "Options :\n" << std::endl;
                std::cout << YELLOW "   -i --input\t" CYAN " -- Path of the input PGN file" RESET << std::endl;
                std::cout << YELLOW "   -o --output\t" CYAN " -- Name of the output TEX file" RESET << std::endl;
                std::cout << YELLOW "   -c --counts\t" CYAN " -- Number of moves before showing the chessboard" RESET << std::endl;

                std::cout << "\nFlags :\n" << std::endl;
                std::cout << YELLOW "   -s --stats\t" CYAN " -- Turn on stat mode" RESET << std::endl;
                std::cout << YELLOW "   -d --debug\t" CYAN " -- Turn on debug mode" RESET << std::endl;
                std::cout << YELLOW "   -h --help\t" CYAN " -- Show help" RESET << std::endl;
                std::cout << YELLOW "   -v --version\t" CYAN " -- Show version\n" RESET << std::endl;

                std::cout << "Example : " PURPLE << argv[0] << BLUE " -i" PURPLE " input.pgn " BLUE "-o" PURPLE " output.tex " BLUE "-c" PURPLE " 5\n" RESET << std::endl;

                exit(EXIT_SUCCESS);
                break;

            case 'v':                                                                                            // -v : affiche la version
                std::cout << "\nBlackLatex " GREEN "version 1.0\n" RESET << std::endl;
                exit(EXIT_SUCCESS);
                break;

            default :
                std::cerr << RED "ERROR: Wrong syntax, use -h to see available options." RESET << std::endl;
                exit(EXIT_FAILURE);
                break;
        }
        
    }




    if (debug) std::cout << CYAN "Start of the main function." RESET << std::endl;



    /// --- TEST DE L'INPUT ---
    // Tester si l'utilisateur n'a pas fourni de path vers l'input
    if(file_input == ""){

        std::cerr << RED"Error : Input file is missing." RESET << std::endl;
        
        exit(EXIT_FAILURE);

    }

    // Insérer l'input dans un stream
    std::ifstream input(file_input);

    // Tester si le fichier existe, et peut être lu
    if (!input.good()) {

        std::cerr << RED"Error : Input file does not exists/can't be read." RESET << std::endl;
        exit(EXIT_FAILURE);

    }

    // Fermer le stream
    input.close();




    /// --- TEST DE L'OUTPUT ---
    // Nom de fichier en sortie
    std::string outname;

    // Si l'option "-o" a été utilisé
    if(output == true){

        // Tester si le nom d'ouput donné contient au moins 4 caractères et contient ".tex" à la fin de celui-ci
        if ( file_output.length() >= 4 && file_output.substr(file_output.length()-4, file_output.length()) == ".tex" ) {

            outname = file_output;

        // Tester si me dernier caractère de l'output donné par l'utilisateur est "/"
        } else if ( file_output.substr(file_output.length()-1, file_output.length()) == "/" ) {

            std::cerr << RED "Error : Output file name is incorrect" RESET << std::endl;

            exit(EXIT_FAILURE);

        // Si il n'y a pas de ".tex" indiqué dans le nom souhaité, rajouter un ".tex" à la fin de celui-ci
        } else {

            outname = file_output + ".tex";

        }

    // Si l'option "-o" n'a pas utilisé, simplement prendre l'input et rajouter ".tex"
    } else {

        file_output = file_input.substr((file_input.find_last_of("/") + 1 ));
        file_output = file_output.substr(0, file_output.length() - 4);
        outname = file_output + ".tex";
    }

    // Teser si l'input est un fichier PGN valide
    PGN checkpgn;
    checkpgn.FromFile(file_input);

    try {

        checkpgn.ParseNextGame();

    // Si la première game n'a pas été trouvé
    } catch (const NoGameFound& e) {

        std::cerr << RED "Error: Input file is not a valid PGN file." RESET << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << YELLOW "File found, attempting conversion. " RESET << std::endl;

    // Insérer le chemin du fichier PGN dans le parser
    PGN pgn;
    pgn.FromFile(file_input);

    // Initialisation du tableau index des NAGs
    std::string NAG[256];

    // Indexation des NAGs avec leurs ID respectif
    NAGindex(NAG);

    std::stringstream buffer;

    // Création de l'entête du document
    buffer
    << "\\documentclass{article}\n"
    << "\\usepackage{multicol}\n"
    << "\\usepackage{array}\n"
    << "\\usepackage{makeidx}\n"
    << "\\usepackage[skaknew]{chessfss}\n"
    << "\\usepackage{texmate}\n"
    << "\\usepackage{xskak}\n"
    << "\\usepackage[top=1.5cm, bottom=2cm, left=1.5cm, right=1cm,headheight=15pt]{geometry}\n"
    << "\\usepackage{adjmulticol}\n"
    << "\\usepackage{fontspec}\n"
    << "\\usepackage{ragged2e}\n\n\n";

    // Si l'affichage du NAG est activé
    buffer << "\\newfontfamily{\\DejaSans}{DejaVu Sans}\n\n\n";

    buffer << "\\begin{document}\n\n";

    // Variable pour compter le nombre de games
    int count = 1;
    
    bool endfile = false;

    while (true) {

        bool strcheck = false;

        // Boucle pour reparser une game si le STR fail
        while (strcheck == false) {

            // On tente d'analyser une game
            try {
                
                strcheck = true;

                pgn.ParseNextGame();

                if (debug) std::cout << "Game n°" << count << ":" <<std::endl;
                count++;

                pgn.STRCheck();

            // Si il n'y a pas de game, fin du fichier atteint
            } catch (const NoGameFound& e) {

                std::cout << RED "No new game found, end of file reached." RESET << std::endl;
                endfile = true;
                break;

            // Si la game ne passe pas le STR check
            } catch (const STRCheckFailed& e) {

                std::cout << RED "Error: Seven Tag Roaster check failed, skipping game..." RESET << std::endl;

                strcheck = false;

            }

        }

        // Si on atteint la fin du fichier
        if (endfile == true) {

            break;

        }

        if (debug) std::cout << PURPLE "I passed the try/catch!" RESET << std::endl;

        // Initiation de la variable qui contient les objets pour les half-moves
        HalfMove *m = new HalfMove();
        pgn.GetMoves(m);

        // Impression des tags STR
        buffer
        << "\\chessevent{" << pgn.GetTagValue("Event") << "}\n"
        << "\\chessopening{" << pgn.GetTagValue("Site") << "}\n\n"
        << "Date : " << pgn.GetTagValue("Date") << "\n\n"
        << "Round : " << pgn.GetTagValue("Round") << "\n\n"
        << "Result : " << pgn.GetResult() << "\n\n"
        << "\\whitename{" << pgn.GetTagValue("White") << "}\n\n"
        << "\\blackname{" << pgn.GetTagValue("Black") << "}\n\n"

        << "\\whiteelo{?}\n\n"
        << "\\blackelo{?}\n\n";

        // Création de la variable tampon pour les tags optionnels
        std::string tag;

        // Tester si le tag ECO existe
        try {

           tag = pgn.GetTagValue("ECO");

        } catch (const InvalidTagName& e) {

            // Ne rien faire si non trouvé

        }

        // Imprimmer le tag ECO s'il existe
        if (!tag.empty()) {

            buffer << "\\ECO{" << tag << "}\n\n";

        }

        // Effacer le contenu de la variable tampon
        tag.clear();

        // Teser si le tag PlyCount existe
        try {

           tag = pgn.GetTagValue("PlyCount");

        } catch (const InvalidTagName& e) {

            // Ne rien faire si non trouvé

        }

        // Imprimmer le tag PlyCount s'il existe
        if (!tag.empty()) {

            buffer << "Plycount : " << tag  << "\n\n";
            
        }

        // Effacer le contenu de la variable tampon
        tag.clear();

        // Insertion de l'entête de la game actuelle
        buffer
        << "\\makegametitle\n"
        << "\\begin{multicols}{2}\n"
        << "\\noindent\n"
        << "\\newchessgame[id=main]\n"
        << "\\xskakset{style=styleC}\n"

        << "\\mainline{";

        int movesnbr = hmperMain*2;                                             // Conversion nombre de count en nombre de half-moves (x2)
        bool secondhalf = false;                                                // Compte du half-move actuel dans le count, contient deux position : "false" et "true" pour représenter le premier et le deuxième half-move du count
        int hmcount = 0;                                                        // Compte du half-move actuel dans la mainline
        bool firstmove = true, mainline_end = false, commenthere = false;       // Valeurs booleans pour les conditions

        if (debug) std::cout << YELLOW "Number of half-moves : " << m->GetLength() << RESET << std::endl;

        for (int i=0 ; i < m->GetLength() ; i++) {

            // Initiation de variable 
            int varORcom = false;

            // Si la mainline est complète
            if (mainline_end == true) {

                hmcount = 1;
                mainline_end = false;

            }

            // Insertion de fin de mainline si la mainline est complète
            if (hmcount == movesnbr) {

                buffer 
                << "}\n"
                << "\\scalebox{0.90}{\\chessboard}\\\\\n"
                << "\\mainline{";

            }

            // Conditions d'affichage du count
            // Si c'est le tout premier half-move de la game, insérer le count
            if (firstmove == true) {

                buffer << m->GetHalfMoveAt(i)->count << ". ";
                firstmove = false;

            // Si on est sur le deuxième half-move du count actuel, insérer le count et réinitialiser le comptage du half-move par count
            } else if (secondhalf == true) {

                buffer << m->GetHalfMoveAt(i)->count << ". ";
                secondhalf = false;

            // Dire qu'on au second half-move
            } else {

                secondhalf = true;

            }

            // Si le nombre de half-move est pas équivaut au nombre maximum que peut contenir une mainline, incrémenter le compte de half-move
            if (hmcount != movesnbr) {

                hmcount++;

            // Si le nombre est équivaut, initier la variable d'insertion de fin de mainline
            } else {

                mainline_end = true;

            }

            // Insertion du half-move dans la mainline
            buffer << m->GetHalfMoveAt(i)->move << " ";

            // Si showNAG est activé
            if (showNAG) {

                // Insertion d'un NAG si présent
                if (m->GetHalfMoveAt(i)->NAG != 0) {

                    buffer << "\\xskakcomment{\\DejaSans\\textcolor{lightgray}{ " << NAG[m->GetHalfMoveAt(i)->NAG] << " \\rmfamily}} ";

                }

            }

            // Insertion de commentaire si présent
            if (!m->GetHalfMoveAt(i)->comment.empty()) {

                varORcom = true;

                buffer
                << "}\n"
                << "\\xskakcomment{\\small\\texttt\\justifying{\\textcolor{darkgray}{" << m->GetHalfMoveAt(i)->comment << "}}}\n";

            }

            /// Insertion de variations
            // Si il y a une variation dans le half-move actuel, initier l'écriture des varations
            if (m->GetHalfMoveAt(i)->variations.size() != 0) {

                // Comptage de variante dans le half-move
                int varcount = 1;

                // Si un commentaire n'as pas été écrit pendant ce half-move, prendre l'initiation de fermer le mainline actuel
                if (varORcom != true) {

                    // Fermer la mainline actuelle
                    buffer << "}\n";            

                // Si présence d'un commentaire, uniquement retourner à la ligne
                } else {

                    buffer << "\n";

                } 

                // Initier la variable de reouverture de mainline pour le prochain half-move
                varORcom = true;                

                // Permet de savoir quand on passe à la deuxième variante si le half-move contient plusieurs varientes
                bool multvar = false;           

                // Boucler à chaque variation
                for (int j=0; j < m->GetHalfMoveAt(i)->variations.size(); j++) {

                    // Si le nombre de variation est supérieur à 1, afficher les numéros de variantes
                    if (m->GetHalfMoveAt(i)->variations.size() > 1) {

                        // Quand arrivé à la deuxième variation, passer à la ligne
                        if ( multvar == true ) {

                            buffer << "\n";

                        }

                        buffer << "\\xskakcomment{\\small\\texttt\\justifying{\\textcolor{gray}{Variation n°" << varcount << ": ";
                        varcount++;

                        multvar = true;

                    // Si il n'y a qu'une seule variation dans le half-move
                    } else {

                            buffer << "\\xskakcomment{\\small\\texttt\\justifying{\\textcolor{gray}{Variation : ";

                    }

                    buffer << "\\variation{" << m->GetHalfMoveAt(i)->variations[j]->count;
                    
                    // Si il s'agit du move blanc
                    if (m->GetHalfMoveAt(i)->variations[j]->isBlack != 1) {

                        buffer << ". ";

                    // S'il s'agit du move noir
                    } else {

                        buffer << "... ";

                    }
                    
                    buffer << m->GetHalfMoveAt(i)->variations[j]->move << " ";

                    if (showNAG) {

                        // Si un NAG est présent, l'insérer...
                        if (m->GetHalfMoveAt(i)->variations[j]->NAG != 0) {

                            buffer << "\\xskakcomment{\\DejaSans\\textcolor{lightgray}{ " << NAG[m->GetHalfMoveAt(i)->variations[j]->NAG] << " \\rmfamily }} ";

                        }

                    }

                    buffer << "} ";

                    // Si il y a un commentaire dans la variation, l'insérer
                    if (!m->GetHalfMoveAt(i)->variations[j]->comment.empty()) {

                        buffer << m->GetHalfMoveAt(i)->variations[j]->comment << " ";

                    }

                    buffer << "}}}\n";

                }

            }

            // Réouverture d'une mainline si la précédente a été interrompue par une variant ou un commentaire
            if (varORcom == true) {

                buffer
                << "\\mainline{";

                // Si ce n'est pas le tout dernier half-move et que le half-move actuel n'est pas le deuxième half-move du count, insérer le count pour reprendre le count incomplet
                if (i != (m->GetLength()-1) && secondhalf != true) {

                    buffer << m->GetHalfMoveAt(i)->count << "... ";
                }

            }

        }

        buffer << "}\n";

        buffer << "\\textbf{Result: ";

        if (m->GetHalfMoveAt(m->GetLength()-1)->isBlack) {

            buffer << "Black ";

        } else {

            buffer << "White ";

        }

        buffer << "wins! (" << pgn.GetResult() << ")}\n";

        buffer << "\\end{multicols}\n";

        buffer << "\\newpage\n\n";

        std::cout << std::endl;

    }
    
    // Fin du document
    buffer << "\\end{document}";
    
    std::ofstream outfile(outname);

    outfile << buffer.str();

    std::cout << GREEN "The file has been successfully converted." RESET << std::endl;

    if (debug) std::cout << CYAN "End of the program." RESET << std::endl;

    return EXIT_SUCCESS;

}

void NAGindex (std::string NAG[256]) {

    NAG[1] = u8"\u0021";
    NAG[2] = u8"\u003F";
    NAG[3] = u8"\u203C";
    NAG[4] = u8"\u2047";
    NAG[5] = u8"\u2049";
    NAG[6] = u8"\u2048";
    NAG[7] = u8"\u25A1";
    NAG[10] = u8"\u003D";
    NAG[13] = u8"\u221E";
    NAG[14] = u8"\u2A72";
    NAG[15] = u8"\u2A71";
    NAG[16] = u8"\u00B1";
    NAG[17] = u8"\u2213";
    NAG[18] = u8"\u002B\u002D";
    NAG[19] = u8"\u002D\u002B";
    NAG[22] = u8"\u2A00";
    NAG[23] = u8"\u2A00";
    NAG[26] = u8"\u25CB";
    NAG[27] = u8"\u25CB";
    NAG[32] = u8"\u27F3";
    NAG[33] = u8"\u27F3";
    NAG[36] = u8"\u2191";
    NAG[37] = u8"\u2191";
    NAG[40] = u8"\u2192";
    NAG[41] = u8"\u2192";
    NAG[44] = u8"\u2BF9";
    NAG[45] = u8"\u2BF9";
    NAG[132] = u8"\u21C6";
    NAG[133] = u8"\u21C6";
    NAG[138] = u8"\u2A01";
    NAG[139] = u8"\u2A01";
    NAG[140] = u8"\u2206";
    NAG[141] = u8"\u2207";
    NAG[142] = u8"\u2313";
    NAG[143] = "<=";
    NAG[144] = "==";
    NAG[145] = "RR";
    NAG[146] = "N";
    NAG[238] = u8"\u25CB";
    NAG[239] = u8"\u21D4";
    NAG[240] = u8"\u21D7";
    NAG[241] = u8"\u229E";
    NAG[242] = u8"\u27EB";
    NAG[243] = u8"\u27EA";
    NAG[244] = u8"\u2715";
    NAG[245] = u8"\u22A5";
    NAG[249] = u8"\u2BFA";
    NAG[250] = u8"\u2BFB";
    NAG[251] = u8"\u2BFC";
    NAG[252] = u8"\u2BFD";
    NAG[254] = u8"\u221F";
    NAG[255] = u8"\u2BFE";

}