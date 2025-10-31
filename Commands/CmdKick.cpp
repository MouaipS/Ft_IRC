#include "CmdKick.hpp"

CmdKick::CmdKick(std::string serverName) : ICommand::ICommand(serverName) {};

void CmdKick::execCmd(
    int fd_origin,
    std::vector<std::string>& cmd,
    const std::string& password,
    std::vector<Channel*>& allChannels,
    std::map<int, User*>& fdToUser
) {
    //Parsing des arguments
    //Chedk si user authentifier
    //Check si il est dans un channel
    //Check si le gars a kick est dans le meme channel
    // Si tout est ok, kick le gars
    //Lister tout les messages possibles
    // TODO: Implémenter la commande Kick

    ////Erreur: "You are not channel operator"
    //::server 481 <nick> :You are not channel operator
    //
    ////Erreur: "No such nick/channel"
    //::server 403 <nick> <channel> :No such nick/channel
    //
    ////Erreur: "They aren't on that channel"
    //::server 441 <nick> <user> :They aren't on that channel
    //
    ////Erreur: "You're not channel operator"
    //::server 482 <nick> <channel> :You're not channel operator
    //
    ////Succès: Expulsion avec message
    //::server KICK <channel> <user> :<message>
    //
    ////Expulsion et application d'un ban
    //::server MODE <channel> +b <user!host>
    //
    ////Erreur: "User not in channel"
    //::server 441 <nick> <user> :They aren't on that channel
    //
    ////Erreur: "Channel is empty"
    //::server 353 <nick> <channel> :Channel is empty


	(void) fd_origin;
	(void) cmd;
	(void) password;
	(void) allChannels;
	(void) fdToUser;	
}
