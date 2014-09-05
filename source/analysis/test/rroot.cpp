// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file tools.license for terms.

//tools_build_use tools inlib csz zlib


#include <tools/args>
#include <tools/fileis>

#include <tools/rroot/file>
#include <tools/rroot/rall>

#ifdef TOOLS_DONT_HAVE_ZLIB
#else
#include <tools/gzip_buffer>
#endif

#include <iostream>
#include <cstdlib>

int main(int argc,char** argv) {


  tools::args args(argc,argv);

  std::string file;
  if(!args.file(file)) {
    std::cout << " give a root file." << std::endl;
    return EXIT_FAILURE;
  }

  bool verbose = args.is_arg("-verbose");
  bool ls = args.is_arg("-ls");
  bool dump = args.is_arg("-dump");

 {bool is;
  tools::file::is_root(file,is);
  if(!is) {
    std::cout << " file is not a root file." << std::endl;
    return EXIT_FAILURE;
  }}

  tools::rroot::file rfile(std::cout,file,verbose);
#ifdef TOOLS_DONT_HAVE_ZLIB
#else
  rfile.add_unziper('Z',tools::gunzip_buffer);
#endif

  if(ls) {
    std::cout << "format version " << rfile.version() << std::endl;
  }
      
  const std::vector<tools::rroot::key*>& keys = rfile.dir().keys();
  tools::rroot::read(std::cout,rfile,keys,true,ls,dump,0);

  ///////////////////////////////////////////////////////////////
  /// if reading the out.root produced with wroot.cpp : /////////
  ///////////////////////////////////////////////////////////////
 {tools::rroot::TDirectory* dir = tools::rroot::find_dir(rfile.dir(),"histo");
  if(dir) {
   {tools::rroot::key* key = dir->find_key("rg");
    if(key) {
      tools::histo::h1d* h = tools::rroot::key_to_h1d(*key);
      if(h) {
        std::cout << "h1d : rg"
                  << ", all_entries " << h->all_entries()
                  << ", entries " << h->entries()
                  << ", mean " << h->mean() << ", rms " << h->rms()
                  << std::endl;
        delete h;
      }
    }}
   {tools::rroot::key* key = dir->find_key("rgbw");
    if(key) {
      tools::histo::h2d* h = tools::rroot::key_to_h2d(*key);
      if(h) {
        std::cout << "h2d : rgbw"
                  << ", all_entries " << h->all_entries()
                  << ", entries " << h->entries()
                  << ", mean_x " << h->mean_x() << ", rms_x " << h->rms_x()
                  << ", mean_y " << h->mean_y() << ", rms_y " << h->rms_y()
                  << std::endl;
        delete h;
      }
    }}
   {tools::rroot::key* key = dir->find_key("prof");
    if(key) {
      tools::histo::p1d* h = tools::rroot::key_to_p1d(*key);
      if(h) {
        std::cout << "p1d : prof"
                  << ", all_entries " << h->all_entries()
                  << ", entries " << h->entries()
                  << ", mean " << h->mean() << ", rms " << h->rms()
                  << std::endl;
        delete h;
      }
    }}
   {tools::rroot::key* key = dir->find_key("prof2D");
    if(key) {
      tools::histo::p2d* h = tools::rroot::key_to_p2d(*key);
      if(h) {
        std::cout << "p2d : prof2D"
                  << ", all_entries " << h->all_entries()
                  << ", entries " << h->entries()
                  << ", mean_x " << h->mean_x() << ", rms_x " << h->rms_x()
                  << ", mean_y " << h->mean_y() << ", rms_y " << h->rms_y()
                  << std::endl;
        delete h;
      }
    }}
  }}

  ///////////////////////////////////////////////////////////////
  /// if reading the pawdemo.root : /////////////////////////////
  ///////////////////////////////////////////////////////////////
 {tools::rroot::key* key = rfile.dir().find_key("h10");
  if(key) {
    tools::histo::h1d* h = tools::rroot::key_to_h1d(*key);
    if(h) {
      std::cout << "h1d : h10"
                << ", all_entries " << h->all_entries()
                << ", entries " << h->entries()
                << ", mean " << h->mean() << ", rms " << h->rms()
                << std::endl;
      delete h;
    }
  }}

  /////////////////////////////////////////////////////////////////////
  /// if reading the prof.root produced with croot_TProfile.cpp : /////
  /////////////////////////////////////////////////////////////////////
 {tools::rroot::key* key = rfile.dir().find_key("prof");
  if(key) {
    tools::histo::p1d* h = tools::rroot::key_to_p1d(*key);
    if(h) {
      std::cout << "p1d : prof"
                << ", all_entries " << h->all_entries()
                << ", entries " << h->entries()
                << ", mean " << h->mean() << ", rms " << h->rms()
                << std::endl;
      delete h;
    }
  }}
 {tools::rroot::key* key = rfile.dir().find_key("prof2D");
  if(key) {
    tools::histo::p2d* h = tools::rroot::key_to_p2d(*key);
    if(h) {
      std::cout << "p2d : prof"
                << ", all_entries " << h->all_entries()
                << ", entries " << h->entries()
                << ", mean_x " << h->mean_x() << ", rms_x " << h->rms_x()
                << ", mean_y " << h->mean_y() << ", rms_y " << h->rms_y()
                << std::endl;
      delete h;
    }
  }}


  return EXIT_SUCCESS;
}
