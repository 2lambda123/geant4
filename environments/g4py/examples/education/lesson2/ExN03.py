#!/usr/bin/python
# ==================================================================
# python script for Geant4Py
#
#   ExN03 : geant4/examples/novice/N03
#        using site-module packages
# ==================================================================
from Geant4 import *
import Qmaterials, NISTmaterials
import ExN03geom
import ExN03pl
import ParticleGun, MedicalBeam
import sys
from time import *

# ==================================================================
# main
# ==================================================================
# ------------------------------------------------------------------
# randum number
# ------------------------------------------------------------------
rand_engine= Ranlux64Engine()
HepRandom.setTheEngine(rand_engine)
HepRandom.setTheSeed(20050830L)

# ------------------------------------------------------------------
# setup for materials
# ------------------------------------------------------------------

# NIST materials
#NISTmaterials.Construct()

# ------------------------------------------------------------------
# setup for geometry
# ------------------------------------------------------------------
# normal way for constructing user geometry


exN03geom= ExN03geom.ExN03DetectorConstruction()
gRunManager.SetUserInitialization(exN03geom)

# 2nd way, short-cut way

#ExN01geom.Construct()
#ExN03geom.Construct()

# magnetic field
#exN03geom.SetMagField(0.1 * tesla)

# ------------------------------------------------------------------
# setup for physics list
# ------------------------------------------------------------------
# normal way for constructing user physics list
exN03PL= ExN03pl.ExN03PhysicsList()
gRunManager.SetUserInitialization(exN03PL)

# 2nd way, short-cut way
#ExN01pl.Construct()
#EMSTDpl.Construct()

# ------------------------------------------------------------------
# setup for primary generator action
# ------------------------------------------------------------------
# normal way for constructing user physics list
#pgPGA= ParticleGun.ParticleGunAction()
#gRunManager.SetUserAction(pgPGA)
#pg= pgPGA.GetParticleGun()

# 2nd way, short-cut way
pg= ParticleGun.Construct()

# set parameters of particle gun
pg.SetParticleByName("e-")
pg.SetParticleEnergy(50.*MeV)
pg.SetParticlePosition(G4ThreeVector(-40.,0.,0.)*cm)
pg.SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.))

# medical beam
#beam= MedicalBeam.Construct()

# ------------------------------------------------------------------
# go...
# ------------------------------------------------------------------
gRunManager.Initialize()

# visualization
gApplyUICommand("/control/execute oglx.mac")

# beamOn
#gRunManager.BeamOn(3)


#TEST
#gProcessTable.SetProcessActivation("msc", 0)
#gProcessTable.SetProcessActivation("conv", 0)
#gProcessTable.SetProcessActivation("eBrem", 0)
#gProcessTable.SetProcessActivation("eIoni", 0)
#gProcessTable.SetProcessActivation("annihil", 0)


# creating widgets using grid layout

from Tkinter import *

class App(Frame):

  
  
  def init(self):

#title and header    row=0, 1
    title = Label(self, text="exampleN03")
    title.grid(row=0, column=1, columnspan=3)
    header = Label(self, text="empowered by \n Geant4Py")
    header.grid(row=1, column=1, columnspan=3)
# number of layers 
    layerLabel = Label(self, bg="green",  text="No of layers")
    self.layerVar=IntVar()
    self.layerVar.set(10)
    layer = Scale(self,  orient=HORIZONTAL, length=400, from_=0, to=10, tickinterval=1, resolution=1, variable=self.layerVar )
    layerLabel.grid(row=2, column=0, sticky=W)
    layer.grid(row=2, column=1, columnspan=5, sticky=W)

#absorber material selection row=3
    absorbermaterialLabel = Label(self, bg="green", text="Absorber Material")
    absorbermaterialLabel.grid(row=3, column=0, sticky=W)
    self.absorbermaterialVar = StringVar()
    self.absorbermaterialVar.set("Lead")
    ra1 = { }
    pos=1
    for i in ("Aluminium", "Lead"):
      ra1[i] = Radiobutton(self, text=i, variable=self.absorbermaterialVar, value=i)
      ra1[i].grid(row=3, column=pos, sticky=W)
      pos=pos+1

#absorber thickness row=4
    absorberthickLabel = Label(self, bg="green", text="Thickness (mm)")
    self.absorberthickVar = DoubleVar()
    self.absorberthickVar.set(10.0)
    absorberthick = Scale(self, orient=HORIZONTAL, length=400, from_=0., to=100., resolution=0.05, tickinterval=10.0, digits=4, variable=self.absorberthickVar)
    absorberthickLabel.grid(row=4, column=0, sticky=W)
    absorberthick.grid(row=4, column=1, columnspan=5, sticky=W)


#gap material selection row=5
    gapmaterialLabel = Label(self, bg="green", text="Gap Material")
    gapmaterialLabel.grid(row=5, column=0, sticky=W)
    self.gapmaterialVar = StringVar()
    self.gapmaterialVar.set("liquidArgon")
    ra2 = { }
    pos=1
    for i in ("liquidArgon","Scintillator", "Air", "Aerogel",  "Galactic" ):
      ra2[i] = Radiobutton(self, text=i, variable=self.gapmaterialVar, value=i)
      ra2[i].grid(row=5, column=pos, sticky=W)
      pos=pos+1

#gap thickness row=6
    gapthickLabel = Label(self, bg="green", text="Thickness (mm)")
    self.gapthickVar = DoubleVar()
    self.gapthickVar.set(5.0)
    gapthick = Scale(self, orient=HORIZONTAL, length=400, from_=0., to=100., resolution=0.05, tickinterval=10.0, digits=4, variable=self.gapthickVar)
    gapthickLabel.grid(row=6, column=0, sticky=W)
    gapthick.grid(row=6, column=1, columnspan=5, sticky=W)

#calorSizeYZ row=7
    calorsizeYZLabel = Label(self, bg="green", text="SizeYZ (mm)")
    self.calorsizeYZVar = DoubleVar()
    self.calorsizeYZVar.set(100.0)
    calorsizeYZ = Scale(self, orient=HORIZONTAL, length=400, from_=0., to=200., resolution=0.05, tickinterval=20.0, digits=4, variable=self.calorsizeYZVar)
    calorsizeYZLabel.grid(row=7, column=0, sticky=W)
    calorsizeYZ.grid(row=7, column=1, columnspan=5, sticky=W)


#particle row=8
    particleLabel = Label(self, bg="green",  text="Particle")
    particleLabel.grid(row=8, column=0, sticky=W)
    self.particleVar = StringVar()
    self.particleVar.set("e-")
    ra1 = { }
    pos1=1
    for i in ("proton", "gamma", "e-",  "e+", "mu-", "mu+"):
      ra1[i] = Radiobutton(self, text=i, variable=self.particleVar, value=i)
      ra1[i].grid(row=8, column=pos1, sticky=W)
      pos1=pos1+1

#energy row=9
    energyLabel = Label(self, bg="green",  text="Energy (MeV)")

    self.energyVar=StringVar()
    self.energyVar.set(50)
    energy = Scale(self, orient=HORIZONTAL, length=400, from_=0., to=1000., tickinterval=100.0, resolution=0.1, variable=self.energyVar, digits=5 )
    energyLabel.grid(row=9, column=0, sticky=W)
    energy.grid(row=9, column=1, columnspan=5, sticky=W)

#number of event row=10 
    eventLabel = Label(self, bg="green",  text="Events")
    self.eventVar=IntVar()
    self.eventVar.set(3)
    event = Scale(self,  orient=HORIZONTAL, length=400, from_=0, to=100, tickinterval=10, resolution=1, variable=self.eventVar )
    eventLabel.grid(row=10, column=0, sticky=W)
    event.grid(row=10, column=1, columnspan=5, sticky=W)

#start a run button row=0
    startBut = Button(self, bg="orange", text="Start a run", command=self.cmd_beamOn)
    startBut.grid(row=0, column=0, sticky=W)

#Zoom in/out Pan X Y row=13
#    visLabel = Label(self, text="viewer", bg="orange")
#    expandBut = Button(self, text="Zoom in", command=self.cmd_expand)
#    shrinkBut = Button(self, text="Zoom out", command=self.cmd_shrink)
#    visLabel.grid(row=13, column=0, sticky=W)
#    expandBut.grid(row=13, column=1, sticky=W)
#    shrinkBut.grid(row=13, column=2, sticky=W)
#    panLabel = Label(self, text="Pan X Y(mm)")
#    self.panXYVar = StringVar()
#    panXYEnt = Entry(self, textvariable=self.panXYVar, width=6)
#    panBut = Button(self, bg="orange", text="OK", command=self.cmd_pan)
#    panLabel.grid(row=13, column=3, sticky=W)
#    panXYEnt.grid(row=13, column=4)
#    panBut.grid(row=13, column=5)

# process activate row 11 - 13
    processLabel=Label(self, text="Process on/off", bg="green")
    processLabel.grid(row=11, column=0, sticky=W)
    procTab = {}
    
    self.processList = ["phot", "compt", "conv", "msc", "eIoni", "eBrem", "annihil","muIoni", "muBrems", "hIoni"]
    pos=1
    self.processVar = {}
    for i in self.processList:
      self.processVar[i] = IntVar()
      procTab[i] = Checkbutton(self, text=i, variable=self.processVar[i], command=self.cmd_setProcess)
      if pos <= 3:
        procTab[i].grid(row=11, column=pos, sticky=W)
      if 4<= pos <= 7:
        procTab[i].grid(row=12, column=pos-3, sticky=W)
      if pos >= 8:
        procTab[i].grid(row=13, column=pos-7, sticky=W)
      pos=pos+1
      procTab[i].select()
# set cuts row 14
    cutLabel = Label(self, bg="green",  text="Cut (mm)")

    self.cutVar=DoubleVar()
    self.cutVar.set(1.)
    cut = Scale(self, orient=HORIZONTAL, length=400, from_=0., to=10., tickinterval=1., resolution=0.005, variable=self.cutVar, digits=5 )
    cutLabel.grid(row=14, column=0, sticky=W)
    cut.grid(row=14, column=1, columnspan=5, sticky=W)

# set mag field row 15
    magLabel = Label(self, bg="green",  text="Magnetic (T)")

    self.magVar=DoubleVar()
    self.magVar.set(0.)
    mag = Scale(self, orient=HORIZONTAL, length=400, from_=0., to=5., tickinterval=1., resolution=0.1, variable=self.magVar, digits=3 )
    magLabel.grid(row=15, column=0, sticky=W)
    mag.grid(row=15, column=1, columnspan=5, sticky=W)


#absorber material selection row=16
    viewerLabel = Label(self, bg="green", text="Viewer")
    viewerLabel.grid(row=16, column=0, sticky=W)
    self.viewerVar = StringVar()
    self.viewerVar.set("OpenGL")
    viewers = { }
    pos=1
    for i in ("OpenGL", "VRML"):
      viewers[i] = Radiobutton(self, text=i, variable=self.viewerVar, value=i, command=self.cmd_viewer)
      viewers[i].grid(row=16, column=pos, sticky=W)
      pos=pos+1


#Geant4 command entry row = 17
    g4comLabel = Label(self, text="Geant4 command", bg="orange")
    self.g4commandVar = StringVar()
    commandEntry = Entry(self, textvariable=self.g4commandVar, width=15)
    comBut = Button(self, bg="orange", text="Execute", command=self.cmd_g4command)
    g4comLabel.grid(row=17, column=0, sticky=W)
    commandEntry.grid(row=17, column=1, columnspan=3, sticky=E+W)
    comBut.grid(row=17, column=5)

#exit row = 0    
    exitBut = Button(self, bg="red", text="End all", command=sys.exit)
    exitBut.grid(row=0, column=5, sticky=W)

#on Run butto do...    
  def cmd_beamOn(self):
      exN03geom.SetNbOfLayers(self.layerVar.get())
      exN03geom.SetAbsorberMaterial(self.absorbermaterialVar.get())
      exN03geom.SetAbsorberThickness(self.absorberthickVar.get()  * mm/2.0)
      exN03geom.SetGapMaterial(self.gapmaterialVar.get())
      exN03geom.SetGapThickness(self.gapthickVar.get()  * mm/2.0)
      exN03geom.SetCalorSizeYZ(self.calorsizeYZVar.get() * mm)
      position = -self.layerVar.get()*(self.absorberthickVar.get() + self.gapthickVar.get())*1.2

      exN03geom.UpdateGeometry()
      exN03PL.SetDefaultCutValue(self.cutVar.get() * mm)
      exN03PL.SetCutsWithDefault()
      exN03geom.SetMagField(self.magVar.get() * tesla)

      print "Now geometry updated"

#      gApplyUICommand("/vis/viewer/flush")
      self.cmd_particle(self.particleVar.get())
      self.cmd_energy(self.energyVar.get())

      print position

      eventNum = self.eventVar.get()
      for i in range(eventNum):

        pg.SetParticlePosition(G4ThreeVector(position*mm, (i-eventNum/2)*5.*mm, 0.*cm))
        gRunManager.BeamOn(1)
        sleep(0.01)
      gApplyUICommand("/vis/viewer/flush")
      
  def cmd_setProcess(self):
    for i in self.processList:
      if self.processVar[i].get() == 0:
         gProcessTable.SetProcessActivation(i, 0)
         print "Process " + i + " inactivated"
      else:
         gProcessTable.SetProcessActivation(i, 1)
         print "Process " + i + " activated"
        
  def cmd_g4command(self):
    gApplyUICommand(self.g4commandVar.get())
      
  def cmd_particle(self, particle):
    gApplyUICommand("/gun/particle " + particle)


  def cmd_energy(self, penergy):
    gApplyUICommand("/gun/energy " + penergy + " MeV")


  def cmd_viewer(self):
    if self.viewerVar.get() == "OpenGL":
#      gApplyUICommand("/vis/scene/notifyHandlers")
      gApplyUICommand("/control/execute oglx.mac")

    if self.viewerVar.get() == "VRML":
      gApplyUICommand("/control/execute vrml.mac")
    

  def cmd_expand(self):
    gApplyUICommand("/vis/viewer/zoom 1.2")

  def cmd_pan(self):
    gApplyUICommand("/vis/viewer/pan " + self.panXYVar.get() + " "  + " mm")


  def cmd_shrink(self):
    gApplyUICommand("/vis/viewer/zoom 0.8")


    
  def __init__(self, master=None):
    Frame.__init__(self, master)
    self.init()
    self.grid()

    
app = App()
app.mainloop()
