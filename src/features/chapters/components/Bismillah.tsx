import React, { useState } from "react";
import { getVerses, getNote } from "../../../shared/scripts";

interface BismillahProps {
  pageIndex: { start: number; end: number };
  chapter: number;
}

export const Bismillah = ({ pageIndex, chapter }: BismillahProps) => {
  const bismillah = getBismillah();
  const [activeBismillahNote, setActiveBismillahNote] = useState<{
    text: string;
  } | null>(null);
  return (
    <>
      {pageIndex.start === 0 && chapter !== 1 && chapter !== 9 && (
        <p className="verse-row">
          <span className="arabic-text">{bismillah?.arabic}</span>
          <span
            className="english-text"
            dangerouslySetInnerHTML={{ __html: bismillah?.english ?? "" }}
            onClick={(e) => {
              const target = e.target as HTMLElement;
              if (target.tagName === "SUP") {
                setActiveBismillahNote((prev) =>
                  prev ? null : { text: getNote(1, 1, 1) },
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

const getBismillah = () => {
  const verses = getVerses(0);
  return verses[0];
};
