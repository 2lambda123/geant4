//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: g4mini.java,v 1.3 2001-07-11 10:09:09 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
import java.io.*;
import java.net.*;

public class g4mini
{
	public static void main( String[] args )
	{
	try{

		// port number 
		final int DEFALUT_PORT_NO = 40801 ;
		int   portNo =  DEFALUT_PORT_NO ;

		// argument checking
		if( args.length != 2 ) 
		{
			System.out.println( "Usage: java g4mini  src_file  server_hostname");
			return ;
		}
		String src         = args[0];
		String server      = args[1];

		// open connection
		Socket   socket    =  new Socket( server, portNo ) ;

		// get input stream from file
		BufferedReader br 
			= new BufferedReader ( new FileReader ( src ) ) ;

		// get output stream to socket
		BufferedWriter bw 
		= new BufferedWriter ( new OutputStreamWriter ( socket.getOutputStream() ) ) ;

		// file ==> socket
		String line ;
		while ( (line = br.readLine()) != null )
		{
			bw.write( line );   
			bw.newLine()    ;
			bw.flush  ()    ;
		}

		// close streams
		br.close();
		bw.close();
		socket.close() ; 
	 }

	catch( Exception e ) 
	{
		System.out.println( e.toString() );
	}	
	} // main
}
