package openocf.r;

// Outbound behaviors (msgs) contain CoObservation payloads

// E.g. a server responds to a GET by creating a CoObservation payload
// containing resource state, and inserts it in an OutboundResponse.

// A client sending an UPDATE creates a CoObservation payload and
// inserts it in an OutboundStimulus.

public class CoObservation {
}
