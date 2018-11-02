#include <stdio.h>
#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";




typedef struct {
    int skt;
} ConnFDP;


/*
 * doit - handle one HTTP request/response transaction
 */
void 
doit(int fd)
{
	int		is_static;
	struct stat	sbuf;
	char		buf       [MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
	char		filename  [MAXLINE], cgiargs[MAXLINE];
	rio_t		rio;

	/* Read request line and headers */
	Rio_readinitb(&rio, fd);
	if (!Rio_readlineb(&rio, buf, MAXLINE))
//line: netp: doit:readrequest
			return;
	printf("%s", buf);
	sscanf(buf, "%s %s %s", method, uri, version);
//line: netp: doit:parserequest
		if (strcasecmp(method, "GET"))
	{
//line: netp: doit:beginrequesterr
			clienterror(fd, method, "501", "Not Implemented",
				    "Tiny does not implement this method");
		return;
} //line: netp: doit:endrequesterr
		read_requesthdrs(&rio);
//line: netp: doit:readrequesthdrs

	/* Parse URI from GET request */
		is_static = parse_uri(uri, filename, cgiargs);//change for proxy, they looked at the url, we can look at the /home.html part becasue it will say instead: http://blahblahblah i.e. doesn't start with /
//line: netp: doit:staticcheck
		if (stat(filename, &sbuf) < 0)
	{
//line: netp: doit:beginnotfound
			clienterror(fd, filename, "404", "Not found",
				    "Tiny couldn't find this file");
		return;
} //line: netp: doit:endnotfound

		if (is_static)//change this to say if its a proxy request, below code changes after copy
	{			/* Serve static content */
		if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode))
		{
	//line: netp: doit:readable
				clienterror(fd, filename, "403", "Forbidden",
					    "Tiny couldn't read the file");
			return;
		}
		serve_static(fd, filename, sbuf.st_size);
//line: netp: doit:servestatic
	} else
	{			/* Serve dynamic content */
		if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode))
		{
	//line: netp: doit:executable
				clienterror(fd, filename, "403", "Forbidden",
				       "Tiny couldn't run the CGI program");
			return;
		}
		serve_dynamic(fd, filename, cgiargs);
//line: netp: doit:servedynamic
	}
}



/* Thread routine */
void *thread(void *vargp)
{
    ConnFDP connfd = *((ConnFDP*)vargp);
    Pthread_detach(pthread_self()); 
    Free(vargp);                    
    doit(connfd.skt);//printf("%i\n",connfd.skt);//replace with doit();
    Close(connfd.skt);
    return NULL;
}

//TODO: erase main after copying tiny in. 269 lines of code
int main(int argc, char** argv)
{
    printf("%s", user_agent_hdr);
    
    int listenfd;
    ConnFDP* connfdp;
    listenfd = Open_listenfd(argv[1]);
    
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;
    
    while (1) {
	clientlen=sizeof(struct sockaddr_storage);
	connfdp = (ConnFDP*) Malloc(sizeof(ConnFDP)); 
	connfdp->skt = Accept(listenfd, (SA *) &clientaddr, &clientlen); 
	Pthread_create(&tid, NULL, thread, connfdp);
    }
    
    return 0;
    
    
}
