import React, { useState } from "react";

interface BismillahProps {
  versesData: {
    arabic: string;
    english: string;
    number: number;
  };
  pageIndex: number;
  chapter: number;
}

export const Bismillah = ({
  versesData,
  pageIndex,
  chapter,
}: BismillahProps) => {
  const bismillah = getBismillah(versesData);
  const [activeBismillahNote, setActiveBismillahNote] = useState<{
    text: string;
  } | null>(null);
  return (
    <>
      {pageIndex.start === 0 && chapter !== 1 && chapter !== 9 && (
        <p className="verse-row">
          <span className="arabic-text">{bismillah.arabic}</span>
          <span
            className="english-text"
            dangerouslySetInnerHTML={{ __html: bismillah.english }}
            onClick={(e) => {
              const target = e.target as HTMLElement;
              if (target.tagName === "SUP") {
                setActiveBismillahNote((prev) =>
                  prev ? null : { text: versesData.getNote(1, 1, 1) },
                );
              }
            }}
          />
        </p>
      )}

      {activeBismillahNote && (
        <div className="footnote-bar">
          <span
            dangerouslySetInnerHTML={{ __html: activeBismillahNote.text }}
          />
          <button onClick={() => setActiveBismillahNote(null)}>×</button>
        </div>
      )}
    </>
  );
};

const getBismillah = (versesData) => {
  const verses = versesData.getVerses(0);
  return verses[0];
};
