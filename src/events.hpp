#ifndef N_EVENTS_H_
#define N_EVENTS_H_

#include <wx/event.h>

wxDECLARE_EVENT(EVT_SURAH_SELECTED, wxCommandEvent);

class NoteClickedEvent : public wxCommandEvent {
 public:
  NoteClickedEvent(wxEventType type, int id, uint surah, uint ayah, uint note)
      : wxCommandEvent(type, id), surah_(surah), ayah_(ayah), note_(note) {}

  uint GetSurah() const { return surah_; }

  uint GetAyah() const { return ayah_; }

  uint GetNote() const { return note_; }

  wxEvent* Clone() const override { return new NoteClickedEvent(*this); }

 private:
  uint surah_;
  uint ayah_;
  uint note_;
};

wxDECLARE_EVENT(EVT_NOTE_CLICKED, NoteClickedEvent);

#endif  // N_EVENTS_H_
