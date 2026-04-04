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
  verseNumber,
  setActiveNotes,
  pushPopup,
}: VerseActivableNotesProps) => {
  return (
    <>
      {activeNotes
        .filter((n) => n.index === verseNumber)
        .map((n) => (
          <div className="footnote-bar" key={`${n.index}-${n.supIndex}`}>
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
                    (p) => !(p.index === n.index && p.supIndex === n.supIndex),
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

const toggleNote = (chapter, verseNumber, supIndex) => (prev) => {
  const exists = prev.find(
    (n) => n.index === verseNumber && n.supIndex === supIndex,
  );
  if (exists) {
    return prev.filter(
      (n) => !(n.index === verseNumber && n.supIndex === supIndex),
    );
  }
  return [
    ...prev,
    {
      index: verseNumber,
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
      index: number;
      supIndex: number;
      text: string;
    }[]
  >([]);

  useEffect(() => {
    setActiveNotes([]);
  }, [popupStack.length]);

  const bodyRef = useRef<HTMLDivElement>(null);
  useEffect(() => {
    if (bodyRef.current) {
      bodyRef.current.scrollTop = 0;
    }
  }, [activeNotes]);
  const overlayRef = useRef<HTMLDivElement>(null);
  useEffect(() => {
    const el = overlayRef.current;
    if (!el) return;

    const prevent = (e: WheelEvent) => {
      const body = bodyRef.current;
      if (!body) return;

      const { scrollTop, scrollHeight, clientHeight } = body;
      const atTop = scrollTop === 0 && e.deltaY < 0;
      const atBottom = scrollTop + clientHeight >= scrollHeight && e.deltaY > 0;

      if (atTop || atBottom) {
        e.preventDefault();
      }
    };

    el.addEventListener("wheel", prevent, { passive: false });
    return () => el.removeEventListener("wheel", prevent);
  }, [popupStack.length]);

  if (popupStack.length === 0) return null;

  const currentPopup = popupStack.at(-1);

  return createPortal(
    <div className="overlay" ref={overlayRef} onClick={popPopup}>
      <div className="reading-popup" onClick={(e) => e.stopPropagation()}>
        <div className="reading-popup-header">
          {popupStack.length > 1 && <button onClick={popPopup}>← back</button>}
          <button onClick={clearPopup}>×</button>
        </div>
        <div className="reading-popup-body" ref={bodyRef}>
          {currentPopup.verses.map((verse, i) => (
            <Fragment key={i}>
              <div className="verse-row">
                <VerseArabic arabic={verse.arabic} />
                <VerseEnglish
                  chapter={currentPopup.surah}
                  verseNumber={verse.number}
                  english={verse.english}
                  setActiveNotes={setActiveNotes}
                />
              </div>
              <VerseActivableNotes
                activeNotes={activeNotes}
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

export const togglePopup = (aIndex) => {
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
    const [ayahBegin, ayahEnd] = ayah.split("-");
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
