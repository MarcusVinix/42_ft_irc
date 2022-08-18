/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Barney e Seus Amigos <B.S.A@student>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 04:07:35 by Barney e Se       #+#    #+#             */
/*   Updated: 2022/08/18 11:52:24 by Barney e Se      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
# define FT_IRC_HPP

# define LISTEN_BACKLOG 50
# define HOST "127.0.0.1"

extern bool	loop;

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <cerrno>
#include <vector>
#include <algorithm>
#include <cctype>
#include <signal.h>
#include <stdexcept>
#include "colors.hpp"

#endif
