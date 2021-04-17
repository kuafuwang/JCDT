Jikes
This area is intended to be a repository of documents of interest to Jikes' hackers. It will included research papers, dissertations, and any other material that seems to be appropriate. Contents remain copyright their authors, and are presented here by permission. We've seeded the collection with a pair of papers from Jikes' original co-author, and IBM researcher Philippe Charles.

A Practical method for Constructing Efficient LALR(k) Parsers with Automatic Error Recovery

Philippe Charles, May 1991

A dissertation in the Department of Computer Science submitted to the faculty of the Graduate School of Arts and Science in partial fulfillment of the requirements for the degree of Doctor of Philosophy at New York University.

[PDF Document]
Philippe Charles' Thesis.pdf
Summary: This thesis details several improvements in the field of automatic parser generation, which are embodied in the JikesPG parser.  Techniques are described for reducing the space requirements of the parser while allowing token lookahead of more than one token, improving the incremental construction of such a parser from a grammar, and for allowing error recovery in the parser while maintaining fast parsing speed for bottom-up parsing. (summary by Eric Blake)

Abstract: LR parsing is used for a wide range of applications, including compiler construction, automatic code generation, language-specific editors and natural language processing. Currently, however, solutions have not been developed for practical multiple-lookahead parsing, fully-automatic error recovery, and space and time-efficient LR parsing across the wide-range of applications.
A practical framework for LR(k) parsing is introduced. An efficient algorithm incrementally constructs an LALR(k) parser with varying-length lookahead strings, whose symbols are consulted during parsing only when necessary. Currently, effective LR error recovery systems require some user intervention. An effective and fully automated syntactic error recovery method for LR(k) parsers is presented. A generally effective method for compressing LR(k) parsing tables is also presented.
These innovations have been incorporated into a parser generator system with automatically produces a production-quality parser with error diagnostics and recovery.


An LR(k) Error Diagnosis and Recovery Method

Philippe Charles, IBM T.J. Watson Research Center
A paper presented at "Second International Workshop on Parsing Technologies"; 13-15 February 1991.
Adobe
    Acrobat Document
conference paper.pdf
Summary: This paper gives an overview of the algorithms that can be used by parsers to have automatic error recovery in minimal space.  The paper details the error recovery elements that allow the parser to detect both simple (one-token) and complex (multi-token) errors without penalizing the parse of a syntactically correct program.  The output of JikesPG provides the necessary structures to use these algorithms effectively. (summary by Eric Blake)

Abstract: In this paper, a new practical, efficient and language-independent syntactic error recovery method for LR(k) parsers is presented. This method is similar to and builds upon the three-level approach of Burke-Fisher. However, it is more time- and space-efficient and fully automatic.


我把它改造成一个库,给JCIDE的3.xx版本使用,JCIDE的3.xx版本使用的代码解析就依赖这个库.