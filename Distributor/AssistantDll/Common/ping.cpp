// ping rountine for *nix 
//
#include "ping.h"

#define	DEFDATALEN	(64-ICMP_MINLEN)	/* default data length */
#define	MAXIPLEN	60
#define	MAXICMPLEN	76
#define	MAXPACKET	(65536 - 60 - ICMP_MINLEN)/* max packet size */

// target: ip address
// time_out: millisecond
bool ping(string target, int time_out)
{

    int s, i, cc, packlen, datalen = DEFDATALEN;
	struct hostent *hp;
	struct sockaddr_in to, from;
	//struct protoent	*proto;
	struct ip *ip;
	u_char *packet, outpack[MAXPACKET];
	char hnamebuf[MAXHOSTNAMELEN];
	string hostname;
	struct icmp *icp;
	int ret, fromlen, hlen;
	fd_set rfds;
	struct timeval tv;
	int retval;
	struct timeval start, end;
	int /*start_t, */end_t;
	bool cont = true;

	to.sin_family = AF_INET;

	// try to convert as dotted decimal address, else if that fails assume it's a hostname
	to.sin_addr.s_addr = inet_addr(target.c_str());
	if (to.sin_addr.s_addr != (u_int)-1)
		hostname = target;
	else 
	{
		hp = gethostbyname(target.c_str());
		if (!hp)
		{
			// cerr << "unknown host "<< target << endl;
			return false;
		}
		to.sin_family = hp->h_addrtype;
		bcopy(hp->h_addr, (caddr_t)&to.sin_addr, hp->h_length);
		strncpy(hnamebuf, hp->h_name, sizeof(hnamebuf) - 1);
		hostname = hnamebuf;
	}
	packlen = datalen + MAXIPLEN + MAXICMPLEN;
	if ( (packet = (u_char *)malloc((u_int)packlen)) == NULL)
	{
		// cerr << "malloc error\n";
		return false;
	}

/*
	if ( (proto = getprotobyname("icmp")) == NULL)
	{
		cerr << "unknown protocol icmp" << endl;
		return false;
	}
*/
	if ( (s = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
	{
		// perror("socket");	/* probably not running as superuser */
		return false;
	}

	icp = (struct icmp *)outpack;
	icp->icmp_type = ICMP_ECHO;
	icp->icmp_code = 0;
	icp->icmp_cksum = 0;
	icp->icmp_seq = 12345;	/* seq and id must be reflected */
	icp->icmp_id = getpid();


	cc = datalen + ICMP_MINLEN;
	icp->icmp_cksum = in_cksum((unsigned short *)icp,cc);

	gettimeofday(&start, NULL);

	i = sendto(s, (char *)outpack, cc, 0, (struct sockaddr*)&to, (socklen_t)sizeof(struct sockaddr_in));
	if ( i < 0 )
	{
		// perror("sendto error");
		return false;
	}
	if ( i != cc )
	{
		// cout << "wrote " << hostname << " " <<  cc << " chars, ret= " << i << endl;
	}
	
	// Watch stdin (fd 0) to see when it has input.
	FD_ZERO(&rfds);
	FD_SET(s, &rfds);

	// select() wait for timeout
	tv.tv_sec  = time_out / 1000;
	tv.tv_usec = time_out % 1000 * 1000; // millisecond to microsecond

	while(cont)
	{
		retval = select(s+1, &rfds, NULL, NULL, &tv);
		if (retval == -1)
		{
			// perror("select()");
			return false;
		}
		else if (retval)
		{
			fromlen = sizeof(sockaddr_in);
			if ( (ret = recvfrom(s, (char *)packet, packlen, 0,(struct sockaddr *)&from, (socklen_t*)&fromlen)) < 0)
			{
				// perror("recvfrom error");
				return false;
			}

			// Check the IP header
			ip = (struct ip *)((char*)packet); 
			hlen = sizeof( struct ip ); 
			if (ret < (hlen + ICMP_MINLEN)) 
			{ 
				// cerr << "packet too short (" << ret  << " bytes) from " << hostname << endl;;
				return false; 
			} 

			// Now the ICMP part 
			icp = (struct icmp *)(packet + hlen); 
			if (icp->icmp_type == ICMP_ECHOREPLY)
			{
				cout << "Recv: echo reply"<< endl;
				if (icp->icmp_seq != 12345)
				{
					// cout << "received sequence # " << icp->icmp_seq << endl;
					continue;
				}
				if (icp->icmp_id != getpid())
				{
					// cout << "received id " << icp->icmp_id << endl;
					continue;
				}
				cont = false;
			}
			else
			{
				// cout << "Recv: not an echo reply" << endl;
				continue;
			}
	
			gettimeofday(&end, NULL);
			end_t = 1000000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
			// cout << "Elapsed time = " << end_t << " usec" << endl;

			return end_t <= time_out * 1000;
		}
		else
		{
			// cout << "No data within one seconds.\n";
			return false;
		}
	}
	return false;
}

uint16_t in_cksum(uint16_t *addr, unsigned len)
{
  uint16_t answer = 0;
  /*
   * Our algorithm is simple, using a 32 bit accumulator (sum), we add
   * sequential 16 bit words to it, and at the end, fold back all the
   * carry bits from the top 16 bits into the lower 16 bits.
   */
  uint32_t sum = 0;
  while (len > 1)  {
    sum += *addr++;
    len -= 2;
  }

  // mop up an odd byte, if necessary
  if (len == 1) {
    *(unsigned char *)&answer = *(unsigned char *)addr ;
    sum += answer;
  }

  // add back carry outs from top 16 bits to low 16 bits
  sum = (sum >> 16) + (sum & 0xffff); // add high 16 to low 16
  sum += (sum >> 16); // add carry
  answer = ~sum; // truncate to 16 bits
  return answer;
}
