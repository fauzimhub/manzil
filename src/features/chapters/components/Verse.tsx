import React, { useState, Fragment, useRef, useEffect } from "react";
import { getNote, getVerse } from "../../../shared/scripts";
import { createPortal } from "react-dom";

interface VerseArabicProps {
  arabic: string;
}

export const VerseArabic = ({ arabic }: VerseArabicProps) => {
  return <span className="arabic-text">{arabic}</span>;
};

interface VerseEnglishProps {
  chapter: number;
  verseNumber: number;
  english: string;
  setActiveNotes: Dispatch<SetStateAction<ActiveNote[]>>;
}

export const VerseEnglish = ({
  chapter,
  verseNumber,
  english,
  setActiveNotes,
}: VerseEnglishProps) => {
  const englishHtml =
    verseNumber !== 0
      ? `<sup class="verse-number">${verseNumber}</sup> ${english}`
      : english;

  return (
    <span
      className="english-text"
      dangerouslySetInnerHTML={{ __html: englishHtml }}
      onClick={(e) => {
        const target = e.target as HTMLElement;
        if (target.tagName === "SUP") {
          if (target.className === "verse-number") return;
          const supIndex = Number(target.textContent);
          setActiveNotes(toggleNote(chapter, verseNumber, supIndex));
        }
      }}
    />
  );
};

interface VerseActivableNotesProps {
  activeNotes: activeNote[];
  chapter: number;
  verseNumber: number;
  setActiveNotes: Dispatch<SetStateAction<activeNote[]>>;
  pushPopup: (aIndex: string) => void;
}

export const VerseActivableNotes = ({
  activeNotes,
  chapter,
  verseNumber,
  setActiveNotes,
  pushPopup,
}: VerseActivableNotesProps) => {
  const noteRef = useRef<HTMLDivElement>(null);
  useEffect(() => {
    noteRef.current?.scrollIntoView({ behavior: "smooth", block: "nearest" });
  }, [activeNotes]);
  return (
    <>
      {activeNotes
        .filter((n) => n.verseIndex === verseNumber && n.surahIndex === chapter)
        .map((n) => (
          <div
            ref={noteRef}
            className="footnote-bar"
            key={`${n.surahIndex}-${n.verseIndex}-${n.supIndex}`}
          >
            <span
              dangerouslySetInnerHTML={{ __html: n.text }}
              onClick={(e) => {
                const target = e.target as HTMLElement;
                if (target.tagName === "A") {
                  const aIndex = target.textContent;
                  const isSingle = /^\d+:\d+$/.test(aIndex);
                  const isRange = /^\d+:\d+-\d+$/.test(aIndex);
                  if (isSingle || isRange) pushPopup(aIndex);
                }
              }}
            />
            <button
              onClick={() =>
                setActiveNotes((prev) =>
                  prev.filter(
                    (p) =>
                      !(
                        p.surahIndex === n.surahIndex &&
                        p.verseIndex === n.verseIndex &&
                        p.supIndex === n.supIndex
                      ),
                  ),
                )
              }
            >
              ×
            </button>
          </div>
        ))}
    </>
  );
};

const toggleNote =
  (chapter: number, verseNumber: number, supIndex: number) => (prev) => {
    const exists = prev.find(
      (n) =>
        n.verseIndex === verseNumber &&
        n.surahIndex === chapter &&
        n.supIndex === supIndex,
    );
    if (exists) {
      return prev.filter(
        (n) =>
          !(
            n.verseIndex === verseNumber &&
            n.surahIndex === chapter &&
            n.supIndex === supIndex
          ),
      );
    }
    return [
      ...prev,
      {
        surahIndex: chapter,
        verseIndex: verseNumber,
        supIndex,
        text: getNote(chapter, verseNumber, supIndex),
      },
    ].sort((a, b) => a.supIndex - b.supIndex);
  };

interface VersePopupProps {
  popupStack: {
    surah: number;
    verses: { arabic: string; english: string; number: number }[];
  }[];
  pushPopup: (aIndex: string) => void;
  popPopup: () => void;
  clearPopup: () => void;
}

export const VersePopup = ({
  popupStack,
  pushPopup,
  popPopup,
  clearPopup,
}: VersePopupProps) => {
  const [activeNotes, setActiveNotes] = useState<
    {
      surahIndex: number;
      verseIndex: number;
      supIndex: number;
      text: string;
    }[]
  >([]);

  /* useEffect(() => {
   *   setActiveNotes([]);
   * }, [popupStack.length]); */

  const currentPopup = popupStack.at(-1);

  const bodyRef = useRef<HTMLDivElement>(null);
  useEffect(() => {
    if (bodyRef.current) {
      bodyRef.current.scrollTop = 0;
    }
  }, [currentPopup]);

  if (popupStack.length === 0) return null;

  return createPortal(
    <div className="overlay" onClick={popPopup}>
      <div className="reading-popup" onClick={(e) => e.stopPropagation()}>
        <div className="reading-popup-header">
          {popupStack.length > 1 && <button onClick={popPopup}>← back</button>}
          <button onClick={clearPopup}>×</button>
        </div>
        <div className="reading-popup-body" ref={bodyRef}>
          {currentPopup?.verses.map((verse, i) => (
            <Fragment key={i}>
              <div className="verse-row">
                <VerseArabic arabic={verse.arabic} />
                <VerseEnglish
                  chapter={currentPopup?.surah}
                  verseNumber={verse.number}
                  english={verse.english}
                  setActiveNotes={setActiveNotes}
                />
              </div>
              <VerseActivableNotes
                activeNotes={activeNotes}
                chapter={currentPopup?.surah}
                verseNumber={verse.number}
                setActiveNotes={setActiveNotes}
                pushPopup={pushPopup}
              />
            </Fragment>
          ))}
        </div>
      </div>
    </div>,
    document.body,
  );
};

export const togglePopup = (aIndex: string) => {
  const isSingle = /^\d+:\d+$/.test(aIndex);
  const isRange = /^\d+:\d+-\d+$/.test(aIndex);
  let versesPopup;
  let surahNum;

  if (isSingle) {
    const [surah, ayah] = aIndex.split(":");
    surahNum = Number(surah);
    versesPopup = getVerse(surahNum - 1, Number(ayah) - 1);
  } else if (isRange) {
    const [surah, ayah] = aIndex.split(":");
    const [ayahBegin, ayahEnd] = ayah?.split("-") ?? [];
    surahNum = Number(surah);
    versesPopup = getVerse(
      surahNum - 1,
      Number(ayahBegin) - 1,
      Number(ayahEnd) - 1,
    );
  }

  return {
    surah: surahNum,
    verses: versesPopup,
  };
};
