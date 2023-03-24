
export const isMousePointerLocked = (): boolean => {

  const lockedElement = (
      document.pointerLockElement ||
      (document as any).mozPointerLockElement ||
      (document as any).webkitPointerLockElement
  );

  return lockedElement !== undefined;
};
