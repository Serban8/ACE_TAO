// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/dds/DataReaderListener_T.h"
#include "dds4ccm/impl/dds/DataWriterListener_T.h"
#include "dds4ccm/impl/dds/Writer_T.h"
#include "dds4ccm/impl/dds/Reader_T.h"
#include "dds4ccm/impl/dds/DataListenerControl_T.h"
#include "dds4ccm/impl/dds/PortStatusListener_T.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Read_T<DDS_TYPE, CCM_TYPE>::DDS_Read_T (void) :
  ccm_dds_reader_ (0)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Read_T<DDS_TYPE, CCM_TYPE>::~DDS_Read_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Read_T<DDS_TYPE, CCM_TYPE>::configuration_complete (
  ::DDS::Topic_ptr topic,
  ::DDS::Subscriber_ptr subscriber,
  const char* library_name,
  const char* profile_name)
{
  DDS4CCM_TRACE ("DDS_Read_T<DDS_TYPE, CCM_TYPE>::configuration_complete");

  try
    {
      if (CORBA::is_nil (this->data_.in ()))
        {
          ::DDS::DataReader_var reader;
          if (profile_name && library_name)
            {
              reader = subscriber->create_datareader_with_profile (
                  topic,
                  library_name,
                  profile_name,
                  ::DDS::DataReaderListener::_nil (),
                  0);
            }
          else
            {
              ::DDS::DataReaderQos drqos;
              reader = subscriber->create_datareader (
                  topic,
                  drqos,
                  ::DDS::DataReaderListener::_nil (),
                  0);
            }
          ::CIAO::DDS4CCM::CCM_DDS_DataReader_i *rd =
            dynamic_cast < ::CIAO::DDS4CCM::CCM_DDS_DataReader_i *> (reader.in ());
          this->ccm_dds_reader_.set_impl (rd->get_impl ());
          this->data_ = ::DDS::CCM_DataReader::_narrow (reader);
          this->dds_read_.set_impl (reader);
          this->dds_read_.set_qos (topic, library_name, profile_name);
        }
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, "DDS_Read_T::configuration_complete: Caught unknown c++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Read_T<DDS_TYPE, CCM_TYPE>::activate (
  ::CCM_DDS::PortStatusListener_ptr listener,
  ACE_Reactor* reactor)
{
  DDS4CCM_TRACE ("DDS_Read_T<DDS_TYPE, CCM_TYPE>::activate");

  try
    {
      if (CORBA::is_nil (this->status_.in ()))
        {
          ACE_NEW_THROW_EX (this->status_,
                            PortStatusListener (listener, reactor),
                            CORBA::NO_MEMORY ());
        }
      this->ccm_dds_reader_.set_listener (
        this->status_.in (),
        ::CIAO::DDS4CCM::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::get_mask (
          listener));
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, "DDS_Read_T::activate: Caught unknown c++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Read_T<DDS_TYPE, CCM_TYPE>::passivate ()
{
  DDS4CCM_TRACE ("DDS_Read_T<DDS_TYPE, CCM_TYPE>::passivate");

  try
    {
      this->ccm_dds_reader_.set_listener (::DDS::DataReaderListener::_nil (), 0);
      this->status_ = ::DDS::DataReaderListener::_nil ();
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, "DDS_Read_T::passivate: Caught unknown c++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Read_T<DDS_TYPE, CCM_TYPE>::remove (
  ::DDS::Subscriber_ptr subscriber)
{
  DDS4CCM_TRACE ("DDS_Read_T<DDS_TYPE, CCM_TYPE>::remove");

  try
    {
      subscriber->delete_datareader (this->data_.in ());
      this->ccm_dds_reader_.set_impl (0);
      this->data_ = ::DDS::CCM_DataReader::_nil ();
      this->dds_read_.set_impl (0);
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, "DDS_Read_T::remove: Caught unknown c++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::reader_type::_ptr_type
DDS_Read_T<DDS_TYPE, CCM_TYPE>::get_data (void)
{
  DDS4CCM_TRACE ("DDS_Read_T<DDS_TYPE, CCM_TYPE>::get_data");

  return &this->dds_read_;
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataReader_ptr
DDS_Read_T<DDS_TYPE, CCM_TYPE>::get_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_Read_T<DDS_TYPE, CCM_TYPE>::get_dds_entity");

  return &this->ccm_dds_reader_;
}
