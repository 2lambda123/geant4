//   $tigre.2@sympatico.ca, louis.archambault@phy.ulaval.ca
//   06/12/02

//*******************************************************
//
// DicomPatientConstructor.cc :
//	- Initialisation of the construction of DICM images
//	- Reading contour information included in Plan.roi
//	  (Region of interest) *** NOT FULLY WORKING YET ***
//	- Definitions are in DicomGeometry.hh
//
// The code was written by :
//	Vincent Hubert-Tremblay at tigre.2@sympatico.ca
//
// For more information contact :
//	Louis Archambault louis.archambault@phy.ulaval.ca
// or
//	Luc Beaulieu beaulieu@phy.ulaval.ca
//
// Centre Hospitalier Universitaire de Quebec (CHUQ),
// Hotel-Dieu de Quebec, departement de Radio-oncologie
// 11 cote du palais. Quebec, QC, Canada, G1R 2J6
// tel (418) 525-4444 #6720
// fax (418) 691 5268
//*******************************************************

#include "DicomPatientConstructor.hh"

#include "globals.hh"
#include "G4String.hh"

G4int DicomPatientConstructor::FindingNbOfVoxels(G4double maxDensity , G4double minDensity)
{
  FILE* lecturePref = G4std::fopen("Data.dat","r");
  FILE* readData;
  char  maxBuffer[300];
  char compressionBuffer[300];
  char name[300];
  G4std::fscanf(lecturePref,"%s",compressionBuffer);
  G4int compression;
  compression = atoi(compressionBuffer);

  G4std::fscanf(lecturePref,"%s",maxBuffer);
  G4int max = atoi(maxBuffer);    
  G4int copyCount = 0;

  for ( G4int i = 1;i <= max;i++ )
    {
      char fullname[300];
      G4std::fscanf(lecturePref,"%s",name);
      G4std::sprintf(fullname,"%s.g4",name);
      readData = G4std::fopen(fullname,"r");
 
      char rowsBuffer[300],columnsBuffer[300];
      G4std::fscanf(readData,"%s %s",rowsBuffer,columnsBuffer);
      G4int rows = atoi(rowsBuffer);
      G4int columns = atoi(columnsBuffer);

      char pixelSpacingXBuffer[300],pixelSpacingYBuffer[300];
      G4std::fscanf(readData,"%s %s",pixelSpacingXBuffer,pixelSpacingYBuffer);
      pixelSpacingX = atof(pixelSpacingXBuffer);
      pixelSpacingY = atof(pixelSpacingYBuffer);
      
      char sliceThicknessBuf[300];
      G4std::fscanf(readData,"%s",sliceThicknessBuf);
      sliceThickness = atoi(sliceThicknessBuf);

      char sliceLocationBuf[300];
      G4std::fscanf(readData,"%s",sliceLocationBuf);
      sliceLocation = atof(sliceLocationBuf);

      G4std::fscanf(readData,"%s",compressionBuffer);
      compression = atoi(compressionBuffer);
      lenr = abs(rows/compression);
      lenc = abs(columns/compression);
      char densityBuffer[300];
      G4std::vector<G4double> density;
      for ( G4int j = 1;j <= lenr;j++ )
        {
	  for ( G4int w = 1;w <= lenc;w++ )
            {
	      if ( G4std::fscanf(readData,"%s",densityBuffer) != -1 )
                {
		  if ( atof(densityBuffer) >= minDensity && atof(densityBuffer) <= maxDensity )
                    {
		      density.push_back( atof(densityBuffer) );
		      copyCount++;
                    }
                }
            }
        }
      G4std::fclose(readData);
    }
  return copyCount;
}
 
