#ifndef XMLHEPREPFACTORY_H
#define XMLHEPREPFACTORY_H 1

#include <string>
#include <iostream>

#include "HEPREP/HepRepReader.h"
#include "HEPREP/HepRepWriter.h"

#include "DefaultHepRepFactory.h"

/**
 *
 * @author M.Donszelmann
 */
class XMLHepRepFactory : public DefaultHepRepFactory {

    public:
        XMLHepRepFactory();
        ~XMLHepRepFactory();

        HEPREP::HepRepReader* createHepRepReader (std::istream* in);
        HEPREP::HepRepReader* createHepRepReader (std::string filename);
        HEPREP::HepRepWriter* createHepRepWriter (std::ostream* out, bool randomAccess, bool compress);
};

#endif
