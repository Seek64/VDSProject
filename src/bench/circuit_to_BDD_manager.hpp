/*=============================================================================
    Written by Carolina P. Nogueira (2016)
=============================================================================*/
#pragma once

#include "bench_circuit_manager.hpp"
#include "../ManagerInterface.h"
#include "Dumper.h"

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>

/* Exceptions */
/**
 * \class DirException
 * \brief This exception should be thrown when it is not possible create a directory.
 *
 */
class DirException : public std::runtime_error {
public:
    DirException(const std::string& message) : std::runtime_error(message) { };
};

/**
 * \class FileException
 * \brief This exception should be thrown when it is not possible open a log file.
 *
 */
class FileException : public std::runtime_error {
public:
    FileException(const std::string& message) : std::runtime_error(message) { };
};
/**
 * \class InexistentBDDLabelException
 * \brief This exception should be thrown when an Inexistent label is called.
 *
 */
class InexistentBDDLabelException : public std::runtime_error {
public:
    InexistentBDDLabelException(const std::string& message) : std::runtime_error(message) { };
};
/**
 * \class InexistentBDD_IDException
 * \brief This exception should be thrown when an invalid BDD_ID is passed as parameter.
 *
 */
class InexistentBDD_IDException : public std::runtime_error {
public:
    InexistentBDD_IDException(const std::string& message) : std::runtime_error(message) { };
};



/**
 * \class circuit_to_BDD_manager
 * 
 * \brief Class to convert circuits nodes into BDD nodes.
 *
 *  Circuit nodes are generated by the class bench_circuit_manager.
 *
 * \authors {Carolina Nogueira}
 * 
 */
class circuit_to_BDD_manager{
    private:
        boost::unordered_map<unique_ID_t,ClassProject::BDD_ID> node2BDDTable; ///< Mapping from circuit node unique IDs to BDD IDs
        std::unordered_map<label_t, ClassProject::BDD_ID> label2BDD_ID_table; ///< Mapping from node's label to BDD IDs
        
        ClassProject::ManagerInterface* BDD_manager;
        std::string result_dir; ///< Directory where the results are stored
        
        /* -------------------
         * Auxiliar Functions
         * -------------------
         */

        /** 
         * \brief returns the BDD_ID of the given circuit ID.
		 * \param node is unique_ID_t
		 * \return ClassProject::BDD_ID
         *  
         */
        ClassProject::BDD_ID circuitId2BDDid(unique_ID_t node);

        /* --------------------------------
         * Boolean Logic Function Handlers
         * --------------------------------
         */

        /** 
         * \brief generates the BDD node equivalent to a variable with label "label".
		 * \param label is label_t
		 * \return ClassProject::BDD_ID
         *  
         */ 
        ClassProject::BDD_ID InputGate(label_t label); 
        
        /** 
         * \brief generates the BDD node equivalent to the NOT gate.
		 * \param node is set_of_circuit_t containing the circuit ID of the gate to be inverted.
		 * \return ClassProject::BDD_ID
         *  
         */ 
        ClassProject::BDD_ID NotGate(set_of_circuit_t node);         
        
        /** 
         * \brief generates the BDD node equivalent to the AND gate.
		 * \param node is set_of_circuit_t containing the circuit IDs of the gates to be used as input.
		 * \return ClassProject::BDD_ID
         *  
         */ 
        ClassProject::BDD_ID AndGate(set_of_circuit_t inputNodes);
        
        /** 
         * \brief generates the BDD node equivalent to the OR gate.
		 * \param node is set_of_circuit_t containing the circuit IDs of the gates to be used as input.
		 * \return ClassProject::BDD_ID
         *  
         */ 
        ClassProject::BDD_ID OrGate(set_of_circuit_t inputNodes);
        
        /** 
         * \brief generates the BDD node equivalent to the NAND gate.
		 * \param node is set_of_circuit_t containing the circuit IDs of the gates to be used as input.
		 * \return ClassProject::BDD_ID
         *  
         */ 
        ClassProject::BDD_ID NandGate(set_of_circuit_t inputNodes);
        
        /** 
         * \brief generates the BDD node equivalent to the NOR gate.
		 * \param node is set_of_circuit_t containing the circuit IDs of the gates to be used as input.
		 * \return ClassProject::BDD_ID
         *  
         */ 
        ClassProject::BDD_ID NorGate(set_of_circuit_t inputNodes);
        
        /** 
         * \brief generates the BDD node equivalent to the XOR gate.
		 * \param node is set_of_circuit_t containing the circuit IDs of the gates to be used as input.
		 * \return ClassProject::BDD_ID
         *  
         */ 
        ClassProject::BDD_ID XorGate(set_of_circuit_t inputNodes);


    public:
        /**
		* \brief Constructor 
		* \param BDD_manager_iface is ClassProject::ManagerInterface
		* Constructor method for the circuit_to_BDD_manager class.
		*
		*/
        circuit_to_BDD_manager(ClassProject::ManagerInterface *BDD_manager_iface);

        /**
		* \brief Destructor 
		* \param none
		* Destructor method for the circuit_to_BDD_manager class.
		*
		*/
        ~circuit_to_BDD_manager();
         
         /** 
          * \brief generates BDD from the circuit nodes provided.
		  * \param circuit is list_of_circuit_t - topologic sorted list containing the circuit nodes
		  * \return none 
          *
          *  Generates the calls to the BDD package in order to  
          *   generate the BDD equivalent to the provided circuit.
          */   
        void GenerateBDD(list_of_circuit_t circuit,std::string benchmark_file);
        
        
		/* --------------------------------
		 *       Printing Functions
		 * --------------------------------
		 */          
        
        void PrintBDD(std::set<label_t> set_of_output_labels);
};   