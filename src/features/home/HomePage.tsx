import React from "react";
import { Link } from "react-router-dom";

import { surahsMetadata } from "../../shared/scripts";

export const HomePage = () => {
  return (
    <ol>
      {surahsMetadata.map((surahMetadata, i) => (
        <li key={i}>
          <Link to={`/chapters?chapter=${i + 1}&page=1`}>
            {surahMetadata.nameTransliteration} ({surahMetadata.nameTranslation}
            )
          </Link>
        </li>
      ))}
    </ol>
  );
};
