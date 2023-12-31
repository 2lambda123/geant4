//***********************************************************************************************************
// Concatenate_BinToStd_ProtonAtExit.C
// Root command file
// Type: root Concatenate_BinToStd_ProtonAtExit.C
//
// It is used in case of interruption
// Read 2 output files ProtonAtExit_1.dat and ProtonAtExit_2.dat that are generated by Geant4
// tomography simulation It reads protons at exit information, and rewrite the events in a binary
// file StimEvent_std.DAT
//
// More information is available in UserGuide
// Created by Z.LI LP2i Bordeaux 2022
//***********************************************************************************************************

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <vector>
// using namespace std;

// Define a structure to read and write each event in the required binary format
struct StimEvent
{
  uint16_t energy_keV;  // different from Pixe Event, it is in keV
  uint16_t pixelIndex;
  uint16_t sliceIndex;
  uint8_t projectionIndex;
};
struct ParticleInfo
{
  float energy_keV;
  float mx;
  float my;
  float mz;
};
struct RunInfo
{
  // uint_16t
  uint8_t projectionIndex;  // 1 byte
  uint16_t sliceIndex;  //
  uint16_t pixelIndex;
  uint32_t nbParticle;  // 4 bytes int
};

struct Point
{
  double m_x;
  double m_y;
  double m_z;
};
bool IsDetected(Point poi1, Point poi2, double theta)
{
  double a = (poi1.m_x * poi2.m_x + poi1.m_y * poi2.m_y + poi1.m_z * poi2.m_z)
             / sqrt(poi1.m_x * poi1.m_x + poi1.m_y * poi1.m_y + poi1.m_z * poi1.m_z)
             / sqrt(poi2.m_x * poi2.m_x + poi2.m_y * poi2.m_y + poi2.m_z * poi2.m_z);
  if (a > 1.0) a = 1;
  if (a < -1.0) a = -1;
  double r = acos(a);
  if (r > theta)
    return false;
  else
    return true;
}

void Recheck()
{
  // Recheck the output file in case
  FILE* input3 = fopen("../build/StimEvent_std_Detector0_Aperture10.2.DAT", "rb");
  StimEvent p;
  double eventId = -1;
  while (fread(&p, 7, 1, input3)) {
    if (p.projectionIndex == 8 && p.sliceIndex == 64 && p.pixelIndex == 10) {
      eventId++;
      printf("StimEvent_%.0f ProjectionIndex=%d, SliceIndex=%d, PixelIndex=%d, Energy_keV=%d keV\n",
             eventId, p.projectionIndex, p.sliceIndex, p.pixelIndex, p.energy_keV);
    }
  }
  fclose(input3);
}
void Concatenate_BinToStd_ProtonAtExit()
{
  // Recheck();
  // return;

  //***********************************************************************
  //**************************Detection parameters (begin)*****************
  //***********************************************************************

  const int nbProjection = 10;
  const int nbSlice = 128;
  const int nbPixel = 20;
  double totalAngleSpan = 180.;  // in degree

  // angle of detector relative to the incident direction of the primary protons at first projection
  // for proton, it is fixed to 0 degree, namely opposite to the source
  double angleOfDetector = 0.;
  double distanceObjectDetector = 22.;  // 22 mm
  double radiusOfDetector = 5.;  // 5 mm
  // double theta = atan(radiusOfDetector/distanceObjectDetector); //half apex angle of the right
  // circular cone in radian
  double theta = 10.2 * TMath::DegToRad();  // in radian

  int P_interrupt = 2;  // Projection of interruption

  //***********************************************************************
  //**************************Detection parameters (end)*******************
  //***********************************************************************

  // assuming there is one interruption
  FILE* input1 = fopen("../build/ProtonAtExit_1.dat", "rb");
  FILE* input2 = fopen("../build/ProtonAtExit_2.dat", "rb");
  FILE* out = fopen("../build/StimEvent_std.DAT", "wb");

  if (input1 == NULL) {
    printf("error for opening the input ProtonAtExit_1.dat file\n");
    return;
  }
  if (input2 == NULL) {
    printf("error for opening the input ProtonAtExit_2.dat file\n");
    return;
  }

  RunInfo runInfo;
  StimEvent stimEvent;
  Point centerOfDetector;
  Point protonMomentum;

  long long count1 = 0;
  long long count2 = 0;
  int runID = -1;  // index of simulations, namely runID, starting from 0

  // ************************************************************(begin)
  // **********************READ FIRST FILE***********************
  // ************************************************************
  while (fread(&runInfo, sizeof(RunInfo), 1, input1)) {
    runID++;
    runInfo.projectionIndex = runID / (nbSlice * nbPixel);
    int remain = runID % (nbSlice * nbPixel);
    runInfo.sliceIndex = remain / nbPixel;
    runInfo.pixelIndex = remain % nbPixel;
    if (runInfo.projectionIndex == P_interrupt) {
      runID--;
      break;
    }

    int nbParticle = runInfo.nbParticle;

    //***********************************************************************
    //**************************Print information (begin)********************
    //***********************************************************************

    printf("-1--runId %d, ProjectionIndex=%d, SliceIndex=%d, PixelIndex=%d, nbParticle = %d\n",
           runID, runInfo.projectionIndex, runInfo.sliceIndex, runInfo.pixelIndex, nbParticle);

    //***********************************************************************
    //**************************Print information (end)**********************
    //***********************************************************************

    if (!nbParticle) continue;
    std::vector<ParticleInfo> protonAtExit(nbParticle);
    fread(&protonAtExit[0], sizeof(ParticleInfo), nbParticle, input1);

    // if(runInfo.sliceIndex!=1) continue;
    // if(runInfo.sliceIndex!=31&&runInfo.sliceIndex!=32) continue;
    // if(runInfo.sliceIndex!=31) continue;

    // angleOfDetector+totalAngleSpan/nbProjection*runInfo.projectionIndex means the angle between
    // source direction and detector, which should be constant when source is rotating
    double ra = TMath::DegToRad()
                * (angleOfDetector + totalAngleSpan / nbProjection * runInfo.projectionIndex);
    centerOfDetector.m_x = distanceObjectDetector * cos(ra);
    centerOfDetector.m_y = distanceObjectDetector * sin(ra);
    centerOfDetector.m_z = 0;

    for (int i = 0; i < nbParticle; ++i) {
      // proton selection: energy should be lower than 4095 keV
      if (protonAtExit[i].energy_keV >= 4095) continue;  // proton selection

      protonMomentum.m_x = protonAtExit[i].mx;
      protonMomentum.m_y = protonAtExit[i].my;
      protonMomentum.m_z = protonAtExit[i].mz;

      if (!IsDetected(centerOfDetector, protonMomentum, theta))
        continue;
      else {
        stimEvent.energy_keV = floor(protonAtExit[i].energy_keV + 0.5);
        stimEvent.projectionIndex = runInfo.projectionIndex;
        stimEvent.sliceIndex = runInfo.sliceIndex;
        stimEvent.pixelIndex = runInfo.pixelIndex;
        fwrite(&stimEvent, 7, 1, out);
        count1++;
      }
    }
  }
  printf("---------------Number of StimEvent in the first file: %lld------------------------\n",
         count1);
  fclose(input1);

  // ************************************************************
  // **********************READ FIRST FILE (end)*****************
  // ************************************************************

  // ************************************************************
  // **********************READ SECOND FILE (begin)**************
  // ************************************************************
  while (fread(&runInfo, sizeof(RunInfo), 1, input2)) {
    runID++;
    runInfo.projectionIndex = runID / (nbSlice * nbPixel);
    int remain = runID % (nbSlice * nbPixel);
    runInfo.sliceIndex = remain / nbPixel;
    runInfo.pixelIndex = remain % nbPixel;

    int nbParticle = runInfo.nbParticle;

    //***********************************************************************
    //**************************Print information (begin)********************
    //***********************************************************************

    printf("-2--runId %d, ProjectionIndex=%d, SliceIndex=%d, PixelIndex=%d, nbParticle = %d\n",
           runID, runInfo.projectionIndex, runInfo.sliceIndex, runInfo.pixelIndex, nbParticle);

    //***********************************************************************
    //**************************Print information (end)**********************
    //***********************************************************************

    if (!nbParticle) continue;

    std::vector<ParticleInfo> protonAtExit(nbParticle);
    fread(&protonAtExit[0], sizeof(ParticleInfo), nbParticle, input2);

    // if(runInfo.sliceIndex!=1) continue;
    // if(runInfo.sliceIndex!=31) continue;
    // if(runInfo.sliceIndex!=31&&runInfo.sliceIndex!=32) continue;

    // angleOfDetector+totalAngleSpan/nbProjection*runInfo.projectionIndex means the angle between
    // source direction and detector, which should be constant when source is rotating
    double ra = TMath::DegToRad()
                * (angleOfDetector + totalAngleSpan / nbProjection * runInfo.projectionIndex);
    centerOfDetector.m_x = distanceObjectDetector * cos(ra);
    centerOfDetector.m_y = distanceObjectDetector * sin(ra);
    centerOfDetector.m_z = 0;

    for (int i = 0; i < nbParticle; ++i) {
      // proton selection: energy should be lower than 4095 keV
      if (protonAtExit[i].energy_keV >= 4095) continue;  // proton selection

      protonMomentum.m_x = protonAtExit[i].mx;
      protonMomentum.m_y = protonAtExit[i].my;
      protonMomentum.m_z = protonAtExit[i].mz;

      if (!IsDetected(centerOfDetector, protonMomentum, theta))
        continue;
      else {
        stimEvent.energy_keV = floor(protonAtExit[i].energy_keV + 0.5);
        stimEvent.projectionIndex = runInfo.projectionIndex;
        stimEvent.sliceIndex = runInfo.sliceIndex;
        stimEvent.pixelIndex = runInfo.pixelIndex;
        fwrite(&stimEvent, 7, 1, out);
        count2++;
      }
    }
  }
  printf("---------------Number of StimEvent in in the second file: %lld------------------------\n",
         count2);

  // ************************************************************
  // **********************READ SECOND FILE (end)****************
  // ************************************************************

  printf("---------------Number of StimEvent in total: %lld------------------------\n",
         count1 + count2);
  fclose(input2);
  fclose(out);
}
